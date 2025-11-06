/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:38:36 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 10:01:33 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Destroys all mutexes and frees allocated memory.
* Called at the end of simulation to clean up resources or if some thread fails to create
* in the middle of the simulation.
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
* Initializes a single philo with their id, forks and meal data.
* And initializes philos' meal times mutex.
*/
static void	init_philo(t_philo *philo, t_simulation *data, int i)
{
	philo->id = i + 1;
	philo->left_fork = &data->fork[i];
	philo->right_fork = &data->fork[(i + 1) % data->philos_num];
	philo->time_of_last_eat = 0;
	philo->times_eaten = 0;
	philo->data = data;
	pthread_mutex_init(&philo->mutex_meal_times, NULL);
}
/*
* Initializes all fork mutexes and allocates fork array.
* Returns 1 on success, 0 on failure.
*/
static int init_forks(t_simulation *data)
{
	int i;

	data->fork = malloc(sizeof(pthread_mutex_t) * data->philos_num);
	if (!data->fork)
		return (0);
	i = 0;
	while (i < data->philos_num)
	{
		pthread_mutex_init(&data->fork[i], NULL);
		i++;
	}
	return (1);
}
/*
* Destroys all forks mutexes and frees fork array.
* Used for cleanup when philo allocation fails.
*/
static void cleanup_forks(t_simulation *data)
{
	int i;

	i = 0;
	while (i < data->philos_num)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	free(data->fork);
}
/*
* Initializes simulation data structure, forks and all philos.
* Sets up starting time and all necessary mutexes.
* Returns pointer to philos array on success, NULL on failure.
*/
t_philo	*init_structs(t_simulation *data)
{
	t_philo			*philos;
	int				i;
	struct timeval	time;

	gettimeofday(&time, NULL);
	data->starting_time = time.tv_sec * 1000 + time.tv_usec / 1000;
	if (!init_forks(data))
		return (NULL);
	philos = malloc(sizeof(t_philo) * data->philos_num);
	if (!philos)
	{
		cleanup_forks(data);
		return (NULL);
	}
	i = 0;
	while (i < data->philos_num)
	{
		init_philo(&philos[i], data, i);
		i++;
	}
	pthread_mutex_init(&data->mutex_print, NULL);
	data->stop_simulation = 0;
	pthread_mutex_init(&data->mutex_stop_simulation, NULL);
	return (philos);
}
