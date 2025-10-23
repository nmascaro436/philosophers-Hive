/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/23 13:32:06 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

 //this routine will be taking forks, eating, sleeping and thinking
static void	*philo_life_routine(void *arg) // each philo does this
{
	
	
}

static void	monitor_routine(void *arg)
{

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
	data->starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000; // converts to milliseconds bboth numbers
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
