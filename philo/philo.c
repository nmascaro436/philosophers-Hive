/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/29 15:23:02 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	void	lonely_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork); // lock the only fork
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d has taken a fork\n", time_since_start(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->mutex_print);
	usleep(philo->data->time_to_die * 1000); // cant eat so we wait until it starves
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
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat * 1000); // way to make the start smoother and to avoid all philos starting at the same time
	while (!is_simulation_over(philo->data) && (philo->data->must_eat_count == -1 // no limit set 
		|| philo->times_eaten < philo->data->must_eat_count))
	{
		think(philo);
		take_forks(philo);
		if (is_simulation_over(philo->data))
			break;
		eat(philo);
		leave_forks(philo);
		sleep_philo(philo);
	}
	return (NULL);
}

// create thread for each philo -> each thread runs the philo_life_routine function
// start the monitoring -> separate thread that keeps checking time of last eat for each philo
// wait for threads to finish (pthread join) -> ends when a philo dies or all philos have eaten the required times
void	start_simulation(t_simulation *data, t_philo *philo)
{
	int i;
	pthread_t *philo_thr;
	pthread_t monitor;
	struct	timeval tv; // stores seconds (since Jan 1 1970) and microseconds (1 second = 1.000.000 microsec)

	i = 0;
	philo_thr = malloc(sizeof(pthread_t) * data->philos_num);
	if (!philo_thr)
		return ;
	gettimeofday(&tv, NULL); // fills struct with current system time
	data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000; // converts to milliseconds both numbers
	while (i < data->philos_num)
	{
		pthread_create(&philo_thr[i], NULL, philo_life_routine, &philo[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, philo);
	i = 0;
	while (i < data->philos_num)
	{
		pthread_join(philo_thr[i], NULL); // waits until philo thread i finishes
		i++;
	}
	pthread_join(monitor, NULL);
	free(philo_thr);
}
