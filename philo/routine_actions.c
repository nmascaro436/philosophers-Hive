/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:14 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 10:37:04 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Calculates and executes appropriate thinking time for philo,
* adapts thinking time based on how close they are to starving.
* Prevents deadlock by managing when philos compete for forks.
*/
void	think(t_philo *philo)
{
	long	think_time;
	long	safe_time_before_death;
	long	time_since_meal;
	long	remaining;

	if (is_simulation_ko(philo->data))
		return ;
	safe_printing_actions(philo, "is thinking");
	pthread_mutex_lock(&philo->mutex_meal_times);
	time_since_meal = time_since_start(philo->data) - philo->time_of_last_eat;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_time_before_death = philo->data->time_to_die
		- (philo->data->time_to_eat + philo->data->time_to_sleep);
	think_time = 0;
	if (safe_time_before_death > 0)
	{
		remaining = philo->data->time_to_die - time_since_meal;
		if (remaining > safe_time_before_death / 2)
			think_time = safe_time_before_death / 2;
	}
	if (think_time > 0)
		usleep (think_time * 1000);
}

/*
* Even philos take right fork first, odd take left first.
* Prevents deadlock by ensuring consistent fork taking order.
*/
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
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

/*
* Philo eats for the specified time duration.
* Updates meal time and count, checks frequently for simulation end.
* Gets the time_of_last_eat at the beginning of the function,
* so the monitor can get the correct value.
*/
void	eat(t_philo *philo)
{
	long	start_action;

	if (is_simulation_ko(philo->data))
		return ;
	pthread_mutex_lock(&philo->mutex_meal_times);
	philo->time_of_last_eat = time_since_start(philo->data);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_printing_actions(philo, "is eating");
	start_action = time_since_start(philo->data);
	while (!is_simulation_ko(philo->data)
		&& (time_since_start(philo->data) - start_action)
		< philo->data->time_to_eat)
		usleep(500);
}

/*
* Philo releases both forks in order based on their id,
* order matches fork acquisition order.
*/
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

/*
* Philo sleeps for the specified time duration.
* Checks frequently for simulation end to allow quick exit.
*/
void	sleep_philo(t_philo *philo)
{
	long	start_action;

	if (is_simulation_ko(philo->data))
		return ;
	safe_printing_actions(philo, "is sleeping");
	start_action = time_since_start(philo->data);
	while (!is_simulation_ko(philo->data)
		&& (time_since_start(philo->data) - start_action)
		< philo->data->time_to_sleep)
		usleep(500);
}
