/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:58:41 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 09:44:22 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Thread_safe check to read flag and determine if simulation has ended.
* Returns the current value of the stop_simulation flag.
*/
int	is_simulation_over(t_simulation *data)
{
	int	val;

	pthread_mutex_lock(&data->mutex_stop_simulation);
	val = data->stop_simulation;
	pthread_mutex_unlock(&data->mutex_stop_simulation);
	return (val);
}

/*
* Thread_safe writer to the stop_simulation flag.
* Sets the flag to the specified value.
 */
void	set_stop_flag(t_simulation *data, int value)
{
	pthread_mutex_lock(&data->mutex_stop_simulation);
	data->stop_simulation = value;
	pthread_mutex_unlock(&data->mutex_stop_simulation);
}

/*
* Checks if all philos have eaten the required number of times.
* Returns 1 if they are satisfied, 0 if at least one isn't.
*/
static int	philos_ate_enough(t_philo *philo, t_simulation *data)
{
	int	i;
	int	times_eaten;

	i = 0;
	while (i < data->philos_num)
	{
		pthread_mutex_lock(&philo[i].mutex_meal_times);
		times_eaten = philo[i].times_eaten;
		pthread_mutex_unlock(&philo[i].mutex_meal_times);
		if (times_eaten < data->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

/*
* Prints death message and sets the stop flag to end the simulation.
* Only prints if the simulation hasn't already stopped.
*/
void	announce_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	if (!is_simulation_over(philo->data))
	{
		printf("%ld %d died\n", time_since_start(philo->data), philo->id);
		set_stop_flag(philo->data, 1);
	}
	pthread_mutex_unlock(&philo->data->mutex_print);
}
/*
* Checks if philo has starved by comparing time since last meal.
* Returns 1 if philo died, 0 otherwise.
*/
static int check_philo_starvation(t_philo *philo, t_simulation *data)
{
	long since_last_meal;

	pthread_mutex_lock(&philo->mutex_meal_times);
	since_last_meal = time_since_start(data) - philo->time_of_last_eat;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	if (since_last_meal > data->time_to_die)
	{	
		announce_death(philo);
		return (1);
	}
	return (0);
}
/*
* Monitor thread routine that checks regularly for philo starvation and
* verifies all have eaten enough. Exits when simulation ends.
*/
void	*monitor_routine(void *arg)
{
	t_philo			*philo;
	int				i;
	t_simulation	*data;
	
	philo = (t_philo *)arg;
	data = philo[0].data;
	while (!is_simulation_over(data))
	{
		i = 0;
		while (i < data->philos_num)
		{
			if (check_philo_starvation(&philo[i], data))
				return (NULL);
			i++;
		}
		if (data->must_eat_count != -1 && philos_ate_enough(philo, data))
			set_stop_flag(data, 1);
		usleep(100);
	}
	return (NULL);
}
