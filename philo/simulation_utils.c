/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:37:23 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/04 11:54:04 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void init_philo_meal_times(t_philo *philo, int philos_num)
{
	int i;

	i = 0;
	while (i < philos_num)
	{
		pthread_mutex_lock(&philo[i].mutex_meal_times); // protected by mutex just for safety, threads not running yet
		philo[i].time_of_last_eat = 0; // All start at time 0, now relative to starting time that we set before
		pthread_mutex_unlock(&philo[i].mutex_meal_times);
		i++;
	}
}

int handle_lonely_philo(t_philo *philo, pthread_t *philo_thr)
{
	if (pthread_create(&philo_thr[0], NULL, philo_life_routine, &philo[0]) != 0)
	{
		free(philo_thr);
		return (0);
	}
	pthread_join(philo_thr[0], NULL);
	free(philo_thr);
	return (1);
}
int create_philo_threads(t_simulation *data, t_philo *philo, pthread_t *philo_thr)
{
	int i;

	i = 0;
	while (i < data->philos_num)
	{
		if (pthread_create(&philo_thr[i], NULL, philo_life_routine, &philo[i]) != 0) // if it fails
		{
			set_stop_flag(data, 1); // set stop flag so if there's some threads running they will exit
			join_threads(philo_thr, i, NULL, 0); // wait for threads already created, monitor not created so NULL and 0
			return (0);
		}
		i++;
	}
	return (i);
}
int create_monitor_thread(t_simulation *data, t_philo *philo, pthread_t *philo_thr, pthread_t *monitor)
{
	if (pthread_create(monitor, NULL, monitor_routine, philo) != 0)
	{
		set_stop_flag(data, 1);
		join_threads(philo_thr, data->philos_num, NULL, 0);
		free(philo_thr);
		return (0);
	}
	return (1);
}
