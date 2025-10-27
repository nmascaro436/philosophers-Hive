/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:41:23 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/27 12:16:50 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation data;
	t_philo *philos;
	struct	timeval tv; // stores seconds (since Jan 1 1970) and microseconds (1 second = 1.000.000 microsec)

	if (argc == 5 || argc == 6)
	{
		validate_input(&data, argv);
		gettimeofday(&tv, NULL); // fills struct with current system time
		data.starting_time = tv.tv_sec * 1000 + tv.tv_usec / 1000; // converts to milliseconds both numbers
		philos = init_structs(&data);
		if (!philos)
			return (1);
		start_simulation(&data, philos);
		cleanup(&data, philos);
	}
	else
		error_and_exit("Missing argument, try again");
	return (0);
}
