/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:29:08 by nmascaro          #+#    #+#             */
/*   Updated: 2025/10/23 10:47:53 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// input is something like ./philo 5(number of philos), 800(time to die), 200(time to eat), 200(time to sleep), 
// 5[number of times philo must eat]

static long	ft_atol(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = 10 * result + (str[i] - '0');
		i++;
	}
	return (result);
}

static int	is_only_positive_digit(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
static int	get_valid_arg(char *str, char *message)
{
	long value;

	value = ft_atol(str);
	if (value == 0 || value > INT_MAX)
		error_and_exit(message);
	return ((int)value);
}

void	validate_input(t_simulation *data, char **argv)
{
	int	i;

	i = 1; // to skip program name
	while (argv[i])
	{
		if (!is_only_positive_digit(argv[i]))
			error_and_exit("Argument must be positive number");
		i++;
	}
	data->philos_num = get_valid_arg(argv[1], "Invalid number of philos");
	data->time_to_die = get_valid_arg(argv[2], "Invalid time_to_die");
	data->time_to_eat = get_valid_arg(argv[3], "Invalid time_to_eat");
	data->time_to_sleep = get_valid_arg(argv[4], "Invalid time_to_sleep");
	if (argv[5])
		data->must_eat_count = get_valid_arg(argv[5], "Invalid count a philo must eat");
	else
		data->must_eat_count = -1;
}
