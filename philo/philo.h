/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 11:37:49 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/04 15:18:28 by nmascaro         ###   ########.fr       */
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
	int					philos_num;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count; //optional argument, if not specified, simulation stops when a philo dies
	long				starting_time;
	pthread_mutex_t		*fork; //array of mutexes (shared resources), one for fork
	pthread_mutex_t		mutex_print; //mutex to avoid overlapping output messages
	int					stop_simulation; // a philo dies or all are full
	pthread_mutex_t		mutex_stop_simulation; // to avoid monitor wanting to write 1 at the same time as philo routine is reading the stop_simulation
}	t_simulation;

typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			time_of_last_eat; //used by monitor to detect starving
	int				times_eaten;
	t_simulation	*data;
	pthread_mutex_t	mutex_meal_times; //to avoid monitor reading time_of_last_eat and times_eaten at the same time as philo is writing
}	t_philo;

int		validate_input(t_simulation *data, char **argv);
void	*monitor_routine(void *arg);
void	*philo_life_routine(void *arg);
int		start_simulation(t_simulation *data, t_philo *philo);
t_philo	*init_structs(t_simulation *data);
long	time_since_start(t_simulation *data);
void	cleanup(t_simulation *data, t_philo *philos);
void	think(t_philo *philo);
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	leave_forks(t_philo *philo);
void	sleep_philo(t_philo *philo);
int		is_simulation_over(t_simulation *data);
void	set_stop_flag(t_simulation *data, int value);
void	init_philo_meal_times(t_philo *philo, int philos_num);
int		handle_lonely_philo(t_philo *philo, pthread_t *philo_thr);
int		create_philo_threads(t_simulation *data, t_philo *philo, pthread_t *philo_thr);
int		create_monitor_thread(t_simulation *data, t_philo *philo, pthread_t *philo_thr, pthread_t *monitor);
void	join_threads(pthread_t *philo, int count, pthread_t *monit, int monit_cr);
void	announce_death(t_philo *philo);
void	safe_printing_actions(t_philo *philo, const char *str);

#endif