/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 10:29:08 by nmascaro          #+#    #+#             */
/*   Updated: 2025/11/06 09:15:04 by nmascaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* Converts a string to a long integer, skiping leading whitespace.
* Returns the converted value.
*/
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
/*
* Checks if a string contains only positive digit chars.
* Returns 1 if valid (only digits), 0 if invalid or empty.
*/
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
/*
* Validates and converts argument string to integer.
* Returns -1 if value is 0 or exceeds INT_MAX.
* Returns the integer value on ssuccess.
*/
static int	get_valid_arg(char *str)
{
	long	value;

	value = ft_atol(str);
	if (value == 0 || value > INT_MAX)
		return (-1);
	return ((int)value);
}
/*
* Parses argument string and stores valid value in destination.
* Prints error message if argument is invalid and returns 0.
* Returns 1 on success.
*/
static int	parse_and_store_arg(int *dest, char *arg, const char *message)
{
	int	value;

	value = get_valid_arg(arg);
	if (value == -1)
	{
		printf("%s\n", message);
		return (0);
	}
	*dest = value;
	return (1);
}

/*
* Validates all arguments and stores them in simulation data.
* Skips the program name, and checks that all arguments are positive and valid.
* Returns 1 if all arguments are valid, 0 if any is invalid.
*/
int	validate_input(t_simulation *data, char **argv)
{
	int	i;

	i = 1;
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
