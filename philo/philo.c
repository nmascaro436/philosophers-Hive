/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/22 15:45:37 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_life_routine(t_philo) // each philo does this
{
	 //this routine will be taking forks, eating, sleeping and thinking
	
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
		pthread_mutex_init(&data->fork[i], NULL);
		i++;
	}
	philos = malloc (sizeof(t_philo) * data->philos_num);
	if (philos)
	{
		free(data->fork);
		return (NULL);
	}
	i = 0;
	while (i < data->philos_num)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = &data->fork[i];
		philos[i].right_fork = &data->fork[(i + 1) % data->philos_num];
		philos[i].time_of_last_eat = data->starting_time;
		philos[i].times_eaten = 0;
		philos[i].data = data;
		i++;
	}
	pthread_mutex_init(&data->mutex_print, NULL);
	data->stop_simulation = 0;
	return (philos);
}
