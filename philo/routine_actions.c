/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 11:37:14 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/27 11:49:20 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d is thinking\n", time_since_start(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->mutex_print);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0) //even
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->mutex_print);
		printf("%ld %d has taken a fork\n", time_since_start(philo->data), philo->id);
		pthread_mutex_unlock(&philo->data->mutex_print);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data->mutex_print);
		printf("%ld %d has taken a fork\n", time_since_start(philo->data), philo->id);
		pthread_mutex_unlock(&philo->data->mutex_print);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->data->mutex_print);
		printf("%ld %d has taken a fork\n", time_since_start(philo->data), philo->id);
		pthread_mutex_unlock(&philo->data->mutex_print);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->mutex_print);
		printf("%ld %d has taken a fork\n", time_since_start(philo->data), philo->id);
		pthread_mutex_unlock(&philo->data->mutex_print);
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d is eating\n", time_since_start(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->mutex_print);
	philo->time_of_last_eat = time_since_start(philo->data);
	philo->times_eaten++;
	usleep(philo->data->time_to_eat * 1000);
}

void	leave_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);
}

void	sleep_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_print);
	printf("%ld %d is sleeping\n", time_since_start(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->mutex_print);
	usleep(philo->data->time_to_sleep * 1000);
}
