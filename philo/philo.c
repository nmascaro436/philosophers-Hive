/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:54 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 09:36:39 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Handles the special case of a single philo.
* Takes one fork, waits for time_to_die and announces death.
*/
static	void	lonely_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_printing_actions(philo, "has taken a fork");
	usleep(philo->data->time_to_die * 1000);
	announce_death(philo);
	pthread_mutex_unlock(philo->left_fork);
}

/*
* Philosopher thread routine that handles philo's life cycle. 
* Odd numbered philos wait before starting to prevent deadlock.
* Runs if simulation is running (no one died) and there's no limit for the meal times or
* until everyone has eaten enough.
* After taking forks, checks again if simulation stopped while taking them,
* so we can release them before stopping.
*/
void	*philo_life_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->philos_num == 1)
	{
		lonely_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 != 0)
		usleep(philo->data->time_to_eat / 2 * 1000);
	while (!is_simulation_over(philo->data) && (philo->data->must_eat_count == -1
			|| philo->times_eaten < philo->data->must_eat_count))
	{
		think(philo);
		take_forks(philo);
		if (is_simulation_over(philo->data))
		{
			leave_forks(philo);
			break ;
		}
		eat(philo);
		leave_forks(philo);
		sleep_philo(philo);
	}
	return (NULL);
}

/*
* Waits for all created philo threads to finish,
* and waits for monitor thread if it was successfully created.
*/
void	join_threads(pthread_t *philo, int count, pthread_t *monit, int monit_cr)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	if (monit_cr)
		pthread_join(*monit, NULL);
}

/*
* Starts the simulation by creating threads and managing execution.
* Handles single philo and multi-philos and frees resources accordingly.
* Returns 0 on failure, and 1 on success.
*/
int	start_simulation(t_simulation *data, t_philo *philo)
{
	pthread_t	*philo_thr;
	pthread_t	monitor;
	int			created_count;

	philo_thr = malloc(sizeof(pthread_t) * data->philos_num);
	if (!philo_thr)
		return (0);
	init_philo_meal_times(philo, data->philos_num);
	if (data->philos_num == 1)
		return (handle_lonely_philo(philo, philo_thr));
	created_count = create_philo_threads(data, philo, philo_thr);
	if (created_count == 0)
	{
		free(philo_thr);
		return (0);
	}
	if (!create_monitor_thread(data, philo, philo_thr, &monitor))
	{
		free(philo_thr);
		return (0);
	}
	join_threads(philo_thr, created_count, &monitor, 1);
	free(philo_thr);
	return (1);
}
