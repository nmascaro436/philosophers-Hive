/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 11:37:49 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/22 11:10:13 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h> 
# include <limits.h> 

typedef struct s_simulation
{
	int	philos_num;
	int	time_to_die; //milliseconds
	int	time_to_eat; // milliseconds
	int	time_to_sleep; // milliseconds
	int	must_eat_count; //optional argument, if not specified, simulation stops when a philo dies
	long	starting_time; // milliseconds
	pthread_mutex_t	*fork; //array of mutexes
	pthread_mutex_t	mutex_print; //mutex to avoid overlapping output messages
	int	stop_simulation; // a philo dies or all are full
}	t_simulation;

typedef struct s_philo
{
	int	id;
	pthread_mutex_t *left_fork;
	pthread_mutex_t	*right_fork;
	long	time_of_last_eat;
	int times_eaten;
	t_simulation *data;
}	t_philo;

#endif