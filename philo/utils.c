/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:38:36 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 10:37:44 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Destroys all mutexes and frees allocated memory.
* Called at the end of simulation to clean up resources
* or if some thread fails to create in the middle of the simulation.
*/
void	cleanup(t_simulation *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philos_num)
	{
		pthread_mutex_destroy(&data->fork[i]);
		pthread_mutex_destroy(&philos[i].mutex_meal_times);
		i++;
	}
	pthread_mutex_destroy(&data->mutex_print);
	pthread_mutex_destroy(&data->mutex_stop_simulation);
	free(data->fork);
	free(philos);
}

/*
* Calculates elapsed time in milliseconds since simulation start.
* Returns the time difference between current time and starting_time.
*/
long	time_since_start(t_simulation *data)
{
	struct timeval	time;
	long			current_time;

	gettimeofday(&time, NULL);
	current_time = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (current_time - data->starting_time);
}

/*
* Thread-safe printing of philo actions with timestamp.
* Only prints if simulation is still running.
*/
void	safe_printing_actions(t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	if (!philo->data->stop_simulation)
		printf("%ld %d %s\n", time_since_start(philo->data), philo->id, str);
	pthread_mutex_unlock(&philo->data->mutex_print);
}

/*
* Prints death message and sets the stop flag to end the simulation.
* Only prints if the simulation hasn't already stopped.
*/
void	announce_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	if (!is_simulation_ko(philo->data))
	{
		printf("%ld %d died\n", time_since_start(philo->data), philo->id);
		set_stop_flag(philo->data, 1);
	}
	pthread_mutex_unlock(&philo->data->mutex_print);
}
