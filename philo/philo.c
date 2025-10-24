/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/24 15:38:48 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// it takes void argument because it's what is expected by pthread create
static void	*philo_life_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100); // way to make the start smoother and to avoid all philos starting at the same time
	while (!philo->data->stop_simulation && (philo->data->must_eat_count == -1 // no limit set 
		|| philo->times_eaten < philo->data->must_eat_count))
	{
		think(philo);
		take_forks(philo);
		eat(philo);
		leave_forks(philo);
		sleep_philo(philo);
	}
	return (NULL);
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

static void	*monitor_routine(void *arg)
{
	t_philo *philo;
	int i;
	long	time;
	t_simulation *data;
	
	philo = (t_philo *)arg;
	data = philo[0].data; // all philos have thee same simulation data, so we use the first one
	while (!data->stop_simulation)
	{
		i = 0;
		while (i < data->philos_num)
		{
			time = time_since_start(data) - philo[i].time_of_last_eat; // time since last meal
			if (time > data->time_to_die) // if starved
			{
				pthread_mutex_lock(&data->mutex_print);
				printf("%ld %d died\n", time_since_start(data), philo[i].id);
				pthread_mutex_unlock(&data->mutex_print);
				data->stop_simulation = 1;
				return (NULL);
			}
			i++;
		}
		if (data->must_eat_count != -1 && philos_ate_enough(philo, data)) // if must eat count is set and all ate enough
			data->stop_simulation = 1;
		usleep(1000); // sleep for 1 millisecond after eah full check of all philos so we dont over bbook CPU
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
	gettimeofday(&tv, NULL); // fills struct with current system time
	data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000; // converts to milliseconds both numbers
	philo_thr = malloc(sizeof(pthread_t) * data->philos_num);
	while (i < data->philos_num)
	{
		pthread_create(&philo_thr[i], NULL, philo_life_routine, &philo[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, data);
	i = 0;
	while (i < data->philos_num)
	{
		pthread_join(philo_thr[i], NULL); // waits until philo thread i finishes
		i++;
	}
	pthread_join(monitor, NULL);
	free(philo_thr);
}
