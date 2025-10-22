/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:38:36 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/22 10:55:59 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_and_exit(const char *message)
{
	printf("%s\n", message);
	exit (EXIT_FAILURE);
}

void	cleanup(t_simulation *data)
{

}
