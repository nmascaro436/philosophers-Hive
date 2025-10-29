/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:41:23 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/29 15:06:05 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation data;
	t_philo *philos;

	if (argc == 5 || argc == 6)
	{
		validate_input(&data, argv);
		
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
