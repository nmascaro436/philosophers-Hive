/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 11:37:49 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/30 14:26:47 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h> 
# include <limits.h> 
# include <unistd.h>

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
	pthread_mutex_t mutex_stop_simulation; // to avoid monitor wanting to write 1 at the same time as philo routine is reading the stop_simulation
}	t_simulation;

typedef struct s_philo
{
	int	id;
	pthread_mutex_t *left_fork;
	pthread_mutex_t	*right_fork;
	long	time_of_last_eat;
	int times_eaten;
	t_simulation *data;
	pthread_mutex_t mutex_meal_times;
}	t_philo;

void	validate_input(t_simulation *data, char **argv);
void	*monitor_routine(void *arg);
void	start_simulation(t_simulation *data, t_philo *philo);
t_philo	*init_structs(t_simulation *data);
long	time_since_start(t_simulation *data);
void	cleanup(t_simulation *data, t_philo *philos);
void	error_and_exit(const char *message);
void	think(t_philo *philo);
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	leave_forks(t_philo *philo);
void	sleep_philo(t_philo *philo);
int	is_simulation_over(t_simulation *data);
void	set_stop_flag(t_simulation *data, int value);

#endif