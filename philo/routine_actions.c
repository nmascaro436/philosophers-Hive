/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:14 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/28 16:37:44 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	safe_printing_actions(t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->data->mutex_stop_simulation);
	if (philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->mutex_stop_simulation);
		return ;
	}
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d %s\n", time_since_start(philo->data), philo->id, str);
	pthread_mutex_unlock(&philo->data->mutex_print);
	pthread_mutex_unlock(&philo->data->mutex_stop_simulation);
}

void	think(t_philo *philo)
{
	safe_printing_actions(philo, "is thinking");
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0) //even
	{
		pthread_mutex_lock(philo->left_fork);
		safe_printing_actions(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		safe_printing_actions(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		safe_printing_actions(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		safe_printing_actions(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	safe_printing_actions(philo, "is eating");
	pthread_mutex_lock(&philo->mutex_meal_times);
	philo->time_of_last_eat = time_since_start(philo->data);
	pthread_mutex_unlock(&philo->mutex_meal_times);
	philo->times_eaten++;
	usleep(philo->data->time_to_eat * 1000);
}

void	leave_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_philo(t_philo *philo)
{
	safe_printing_actions(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
}
