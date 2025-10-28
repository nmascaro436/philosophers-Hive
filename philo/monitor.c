/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:58:41 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/28 16:38:34 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_simulation_over(t_simulation *data)
{
	int val;

	pthread_mutex_lock(&data->mutex_stop_simulation);
	val = data->stop_simulation;
	pthread_mutex_unlock(&data->mutex_stop_simulation);
	return (val);
}

void	set_stop_flag(t_simulation *data, int value)
{
	pthread_mutex_lock(&data->mutex_stop_simulation);
	data->stop_simulation = value;
	pthread_mutex_unlock(&data->mutex_stop_simulation);
}
static int	philos_ate_enough(t_philo *philo, t_simulation *data)
{
	int i;

	i = 0;
	while (i < data->philos_num)
	{
		if (philo[i].times_eaten < data->must_eat_count)
			return (0); // at least one didnt eat enough
		i++;
	}
	return (1); // all ate enough
}

void	*monitor_routine(void *arg)
{
	t_philo *philo;
	int i;
	long	time;
	t_simulation *data;
	
	philo = (t_philo *)arg;
	data = philo[0].data; // all philos have thee same simulation data, so we use the first one
	while (!is_simulation_over(data))
	{
		i = 0;
		while (i < data->philos_num)
		{
			pthread_mutex_lock(&philo[i].mutex_meal_times);
			time = time_since_start(data) - philo[i].time_of_last_eat; // time since last meal
			pthread_mutex_unlock(&philo[i].mutex_meal_times);
			if (time >= data->time_to_die) // if starved, not using the safe print because it needs to print even while setting the stop flag
			{
				pthread_mutex_lock(&data->mutex_print);
				printf("%ld %d died\n", time_since_start(data), philo[i].id);
				pthread_mutex_unlock(&data->mutex_print);
				set_stop_flag(data, 1);
				return (NULL);
			}
			i++;
		}
		if (data->must_eat_count != -1 && philos_ate_enough(philo, data)) // if must eat count is set and all ate enough
			set_stop_flag(data, 1);
		usleep(500); // sleep for 1 millisecond after each full check of all philos so we dont over book CPU
	}
	return (NULL);
}
