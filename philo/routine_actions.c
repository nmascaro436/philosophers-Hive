/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:14 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/30 16:24:05 by nmascaro         ###   ########.fr       */
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
	if (!is_simulation_over(philo->data))
		safe_printing_actions(philo, "is thinking");
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
	//pthread_mutex_lock(&philo->mutex_meal_times);
	//philo->time_of_last_eat = time_since_start(philo->data);
	//pthread_mutex_unlock(&philo->mutex_meal_times);
}

void	eat(t_philo *philo)
{
	long	start_action;
	
	if (is_simulation_over(philo->data))
		return ;
	start_action = time_since_start(philo->data); // when philo started eating
	pthread_mutex_lock(&philo->mutex_meal_times);
	philo->time_of_last_eat = start_action;
	philo->times_eaten++;
	pthread_mutex_unlock(&philo->mutex_meal_times);
	safe_printing_actions(philo, "is eating");
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

