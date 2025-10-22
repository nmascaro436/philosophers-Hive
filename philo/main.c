/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:41:23 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/22 12:34:35 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation data;

	if (argc == 5 || argc == 6)
	{
		validate_input(&data, argv);
		init_structs(&data);
		start_simulation(&data);
		cleanup(&data);
	}
	else
		error_and_exit("Missing argument, try again");
}
