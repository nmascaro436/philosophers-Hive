/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:38:36 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/28 16:43:17 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_and_exit(const char *message)
{
	printf("%s\n", message);
	exit (EXIT_FAILURE);
}

void	cleanup(t_simulation *data, t_philo *philos)
{
	int i;

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
long	time_since_start(t_simulation *data)
{
	struct timeval tv;
	long	current_time;

	gettimeofday(&tv, NULL);
	current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (current_time - data->starting_time);
}
t_philo	*init_structs(t_simulation *data)
{
	t_philo	*philos;
	int	i;

	i = 0;
	data->fork = malloc(sizeof(pthread_mutex_t) * data->philos_num);
	if (!data->fork)
		return (NULL);
	while (i < data->philos_num)
	{
		pthread_mutex_init(&data->fork[i], NULL); // NULL because we want the default behavior of a mutex
		i++;
	}
	philos = malloc(sizeof(t_philo) * data->philos_num);
	if (!philos)
	{
		free(data->fork);
		return (NULL);
	}
	i = 0;
	while (i < data->philos_num)
	{
		philos[i].id = i + 1; // we start from philo number 1
		philos[i].left_fork = &data->fork[i]; // the one that matches their index
		philos[i].right_fork = &data->fork[(i + 1) % data->philos_num]; // we use modulo so the last one can get the fork of the first philo (p.ex: 4 % 4 is 0)
		philos[i].time_of_last_eat = data->starting_time;
		philos[i].times_eaten = 0;
		philos[i].data = data;
		pthread_mutex_init(&philos[i].mutex_meal_times, NULL);
		i++;
	}
	pthread_mutex_init(&data->mutex_print, NULL); // ensures only one thread prints at a time, no overlap
	data->stop_simulation = 0;
	pthread_mutex_init(&data->mutex_stop_simulation, NULL);
	return (philos);
}
