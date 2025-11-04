/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:14 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/04 15:25:47 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_printing_actions(t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	if (!philo->data->stop_simulation)
		printf("%ld %d %s\n", time_since_start(philo->data), philo->id, str);
	pthread_mutex_unlock(&philo->data->mutex_print);
}

// adapts thinking time depending on how close they are to starving
//  if there's a lot of philos even if there's a wait time for the odd even numbers, even them
// may try to take forks at the same time, so then here we smartly decide which one gets to have forks first by not thinking
// also gives time for others to finish eating and release forks
void	think(t_philo *philo)
{
	long	think_time;
	long	safe_time_before_death; // how much time they have before starving
	long	time_since_meal; // when it last ate
	long	remaining; // time remaining before death

	if (is_simulation_over(philo->data))
		return ;
	safe_printing_actions(philo, "is thinking");
	pthread_mutex_lock(&philo->mutex_meal_times);
	time_since_meal = time_since_start(philo->data) - philo->time_of_last_eat;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_time_before_death = philo->data->time_to_die - (philo->data->time_to_eat
			+ philo->data->time_to_sleep);
	think_time = 0;
	if (safe_time_before_death > 0) // if there's buffer time, we calculate think time, if not think time stays at 0 (they print is thinking but immediately do something else)
	{
		remaining = philo->data->time_to_die - time_since_meal; // time until death
		if (remaining > safe_time_before_death / 2) // if the remaining is half of safe buffer
			think_time = safe_time_before_death / 2; // they think for that half
	}
	if (think_time > 0)
		usleep (think_time * 1000); // wait for think time in miliseconds
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0) //even
	{
		pthread_mutex_lock(philo->right_fork);
		safe_printing_actions(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		safe_printing_actions(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		safe_printing_actions(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		safe_printing_actions(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	long	start_action;

	if (is_simulation_over(philo->data))
		return ;
	pthread_mutex_lock(&philo->mutex_meal_times);
	philo->time_of_last_eat = time_since_start(philo->data); // the last moment the philo started eating, used by monitor so it doesnt think it hasnt eaten
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_printing_actions(philo, "is eating");
	start_action = time_since_start(philo->data); // moment it began the eating action
	while (!is_simulation_over(philo->data)
		&& (time_since_start(philo->data) - start_action) < philo->data->time_to_eat)// checks frequently whether the simulation stopped or the time to eat is done
		usleep(500); 
}

void	leave_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

void	sleep_philo(t_philo *philo)
{
	long start_action;

	if (is_simulation_over(philo->data))
		return ;
	safe_printing_actions(philo, "is sleeping");
	start_action = time_since_start(philo->data);
	while (!is_simulation_over(philo->data) 
		&& (time_since_start(philo->data) - start_action) < philo->data->time_to_sleep)
		usleep(500); // sleep in chunks to let the monitor thread check for starving
}
