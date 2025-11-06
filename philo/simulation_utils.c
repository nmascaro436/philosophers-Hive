/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:37:23 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 10:34:00 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Initializes all philos' last meal time to 0 (simulation start).
* Called before creating threads to ensure consistent starting time.
*/
void	init_philo_meal_times(t_philo *philo, int philos_num)
{
	int	i;

	i = 0;
	while (i < philos_num)
	{
		pthread_mutex_lock(&philo[i].mutex_meal_times);
		philo[i].time_of_last_eat = 0;
		pthread_mutex_unlock(&philo[i].mutex_meal_times);
		i++;
	}
}

/*
* Handles simulation with single philo.
* Creates thread, waits for completion and frees resources.
* Returns 1 on success, 0 on failure of thread creation.
*/
int	handle_lonely_philo(t_philo *philo, pthread_t *philo_thr)
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

/*
* Creates all philo threads. If any creation fails, sets stop flag,
* and waits for already created threads.
* Returns number of successfully created threads.
*/
int	create_philo_threads(t_simulation *data, t_philo *ph, pthread_t *ph_thr)
{
	int	i;

	i = 0;
	while (i < data->philos_num)
	{
		if (pthread_create(&ph_thr[i], NULL, philo_life_routine, &ph[i]) != 0)
		{
			set_stop_flag(data, 1);
			join_threads(ph_thr, i, NULL, 0);
			return (0);
		}
		i++;
	}
	return (i);
}

/*
* Creates monitor thread. If creation fails, stops simulation
* and waits for philo threads.
* Returns 1 on success, 0 on failure.
*/
int	monit_thr(t_simulation *dat, t_philo *ph, pthread_t *ph_thr, pthread_t *mon)
{
	if (pthread_create(mon, NULL, monitor_routine, ph) != 0)
	{
		set_stop_flag(dat, 1);
		join_threads(ph_thr, dat->philos_num, NULL, 0);
		free(ph_thr);
		return (0);
	}
	return (1);
}
