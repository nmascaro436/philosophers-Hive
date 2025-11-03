/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:14 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/03 09:58:25 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_printing_actions(t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->data->mutex_stop_simulation); // protect the variable 
	if (philo->data->stop_simulation) // if someone died or everyone finished eating
	{
		pthread_mutex_unlock(&philo->data->mutex_stop_simulation);
		return ; // exit so no more prints happen
	}
	pthread_mutex_unlock(&philo->data->mutex_stop_simulation);
	pthread_mutex_lock(&philo->data->mutex_print);  // simulation not over so we can print
	printf("%ld %d %s\n", time_since_start(philo->data), philo->id, str);
	pthread_mutex_unlock(&philo->data->mutex_print);

}

void	think(t_philo *philo)
{
	long think_time;
	long safe_time_before_death;
	long time_since_meal;
	long remaining;
	
	if (is_simulation_over(philo->data))
		return ;
	safe_printing_actions(philo, "is thinking");
	pthread_mutex_lock(&philo->mutex_meal_times);
	time_since_meal = time_since_start(philo->data) - philo->time_of_last_eat;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_time_before_death = philo->data->time_to_die - (philo->data->time_to_eat
	+ philo->data->time_to_sleep);
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
	philo->time_of_last_eat = time_since_start(philo->data);
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_printing_actions(philo, "is eating");
	start_action = time_since_start(philo->data); // when philo started eating
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
