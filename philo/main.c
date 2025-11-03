/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:41:23 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/03 10:01:43 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation data;
	t_philo *philos;

	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments, try again\n");
		return (1);
	}
	if (!validate_input(&data, argv))
		return (1);
	philos = init_structs(&data);
	if (!philos)
		return (1);
	if (!start_simulation(&data, philos))
	{
		cleanup(&data, philos);
		return (1);
	}
	cleanup(&data, philos);
	return (0);
}
