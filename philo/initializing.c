/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:34:28 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/10 12:04:46 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
static int	init_forks(t_simulation *data)
{
	int	i;

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
static void	cleanup_forks(t_simulation *data)
{
	int	i;

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
	return (philos);
}
