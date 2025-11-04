/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/04 11:27:06 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	void	lonely_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork); // lock the only fork
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d has taken a fork\n", time_since_start(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->mutex_print);
	usleep(philo->data->time_to_die * 1000); // cant eat so we wait until it starves, usleep uses microseconds so we make it to milliseconds
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d died\n", time_since_start(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->mutex_print);
	set_stop_flag(philo->data, 1);
	pthread_mutex_unlock(philo->left_fork);
}

// it takes void argument because it's what is expected by pthread create
static void	*philo_life_routine(void *arg)
{
	t_philo *philo;
	
	philo = (t_philo *)arg;
	if (philo->data->philos_num == 1)
	{
		lonely_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 != 0) // odd number of philos
		usleep(philo->data->time_to_eat / 2 * 1000); // wait for half of the eating time(way to make the start smoother and to avoid all philos starting at the same time)
	while (!is_simulation_over(philo->data) && (philo->data->must_eat_count == -1 // no limit set 
		|| philo->times_eaten < philo->data->must_eat_count)) // haven't eaten enough yet
	{
		think(philo);
		take_forks(philo);
		if (is_simulation_over(philo->data)) // check again if simulation stopped while taking forks
		{
			leave_forks(philo); // maybe a philo died while waiting for forks, so we should release them before stoping the simulation
			break ;
		}
		eat(philo);
		leave_forks(philo);
		sleep_philo(philo);
	}
	return (NULL);
}
static void	join_threads(pthread_t *philo, int count, pthread_t *monit, int monit_cr)
{
	int	i;

	i = 0;
	while(i < count)
	{
		pthread_join(philo[i], NULL); // only wait for threads successfully created
		i++;
	}
	if (monit_cr)
		pthread_join(*monit, NULL);
}

// create thread for each philo -> each thread runs the philo_life_routine function
// start the monitoring -> separate thread that keeps checking time of last eat for each philo
// wait for threads to finish (pthread join) -> ends when a philo dies or all philos have eaten the required times
int	start_simulation(t_simulation *data, t_philo *philo)
{
	int i;
	pthread_t *philo_thr;
	pthread_t monitor;
	int created_count;

	philo_thr = malloc(sizeof(pthread_t) * data->philos_num);
	if (!philo_thr)
		return (0);
	i = 0;
	while (i < data->philos_num)
	{
		pthread_mutex_lock(&philo[i].mutex_meal_times); // protected by mutex just for safety, threads not running yet
		philo[i].time_of_last_eat = 0; // All start at time 0, now relative to starting time that we set before
		pthread_mutex_unlock(&philo[i].mutex_meal_times);
		i++;
	}
	if (data->philos_num == 1)
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
	created_count = 0;
	while (created_count < data->philos_num)
	{
		if (pthread_create(&philo_thr[created_count], NULL, philo_life_routine, &philo[created_count]) != 0) // if it fails
		{
			set_stop_flag(data, 1); // set stop flag so if there's some threads running they will exit
			join_threads(philo_thr, created_count, NULL, 0); // wait for threads already created, monitor not created so NULL and 0
			return (0);
		}
		created_count++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, philo) != 0)
	{
		set_stop_flag(data, 1);
		join_threads(philo_thr, created_count, NULL, 0);
		free(philo_thr);
		return (0);
	}
	join_threads(philo_thr, created_count, &monitor, 1); // if success also wait for monitor to finish
	free(philo_thr);
	return (1);
}
