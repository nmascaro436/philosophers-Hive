/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:58:41 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/04 15:26:50 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_simulation_over(t_simulation *data) // reads the flag to see if the simulation ended 
{
	int	val;

	pthread_mutex_lock(&data->mutex_stop_simulation);
	val = data->stop_simulation;
	pthread_mutex_unlock(&data->mutex_stop_simulation);
	return (val);  // return value we read
}

void	set_stop_flag(t_simulation *data, int value) // writes to the flag 
{
	pthread_mutex_lock(&data->mutex_stop_simulation);
	data->stop_simulation = value;
	pthread_mutex_unlock(&data->mutex_stop_simulation);
}

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
			return (0); // at least one didnt eat enough
		i++;
	}
	return (1); // all ate enough
}

void	announce_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	if (!is_simulation_over(philo->data))
	{
		printf("%ld %d died\n", time_since_start(philo->data), philo->id);
		set_stop_flag(philo->data, 1); 	// stop the simulation so other threads exit
	}
	pthread_mutex_unlock(&philo->data->mutex_print);
}
void	*monitor_routine(void *arg)
{
	t_philo			*philo;
	int				i;
	long			since_last_meal;
	t_simulation	*data;
	
	philo = (t_philo *)arg;
	data = philo[0].data; // all philos have thee same simulation data, so we use the first one
	while (!is_simulation_over(data))
	{
		i = 0;
		while (i < data->philos_num)
		{
			pthread_mutex_lock(&philo[i].mutex_meal_times);
			since_last_meal = time_since_start(data) - philo[i].time_of_last_eat; // time since last meal
			pthread_mutex_unlock(&philo[i].mutex_meal_times);
			if (since_last_meal > data->time_to_die) // if starved, not using the safe print because it's the one that needs to set the flag
			{
				announce_death(&philo[i]);
				return (NULL);
			}
			i++;
		}
		if (data->must_eat_count != -1 && philos_ate_enough(philo, data)) // if must eat count is set and all ate enough
			set_stop_flag(data, 1);
		usleep(100); // sleep after each full check of all philos so we dont over book CPU, but still checks frequently
	}
	return (NULL);
}
