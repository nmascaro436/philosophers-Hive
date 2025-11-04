# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nmascaro <nmascaro@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/17 11:34:12 by nmascaro          #+#    #+#              #
#    Updated: 2025/11/04 11:59:58 by nmascaro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = input_validation.c main.c philo.c routine_actions.c utils.c monitor.c simulation_utils.c
OBJS = $(SRCS:.c=.o)

.SECONDARY: $(OBJS)

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	@rm -f *.o
	
fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re