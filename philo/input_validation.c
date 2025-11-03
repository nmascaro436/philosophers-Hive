/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:29:08 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/03 09:42:45 by nmascaro         ###   ########.fr       */
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
static int	get_valid_arg(char *str)
{
	long value;

	value = ft_atol(str);
	if (value == 0 || value > INT_MAX)
		return (-1);
	return ((int)value);
}
static int	parse_and_store_arg(int *dest, char *arg, const char *message)
{
	int value;

	value = get_valid_arg(arg);
	if (value == -1)
	{
		printf("%s\n", message);
		return (0);
	}
	*dest = value;
	return (1);
}
int	validate_input(t_simulation *data, char **argv)
{
	int	i;

	i = 1; // to skip program name
	while (argv[i])
	{
		if (!is_only_positive_digit(argv[i]))
		{
			printf("Argument must be positive number\n");
			return (0);
		}
		i++;
	}
	if (!parse_and_store_arg(&data->philos_num, argv[1], "Invalid number of philos"))
		return (0);
	if (!parse_and_store_arg(&data->time_to_die, argv[2], "Invalid time_to_die"))
		return (0);
	if (!parse_and_store_arg(&data->time_to_eat, argv[3], "Invalid time_to_eat"))
		return (0);
	if (!parse_and_store_arg(&data->time_to_sleep, argv[4], "Invalid time_to_sleep"))
		return (0);
	if (argv[5])
	{
		if (!parse_and_store_arg(&data->must_eat_count, argv[5], "Invalid must_eat_count"))
			return (0);
	}
	else
		data->must_eat_count = -1;
	return (1);
}
