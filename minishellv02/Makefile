# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/26 12:48:55 by kevwang           #+#    #+#              #
#    Updated: 2025/08/12 18:03:14 by vafavard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -Wall -Wextra -Werror

FLAGS = -lreadline

NAME = minishell

# FILE = main.c \
# ft_split.c \
# ft_strjoin.c \
# minishell_exec.c \
# minishell_free.c \
# minishell_utils.c

FILE = parsing_test.c \
pipex_path.c \
parsing_dollar.c \
minishell_utils.c \
ft_strjoin.c \
ft_split.c \
ft_itoa.c \
parsing_lst_utils.c \
parsing_free.c \
parsing_double_tab.c \
parsing_triple_tab.c \
exec_pipe.c \
parsing_parsing_0.c \
parsing_parsing_1.c \
parsing_parsing_2.c \
parsing_parsing_3.c \
parsing_redir.c \
parsing_lst_utils_2.c \
parsing_init.c \
cd_test_4.c \
parsing_concatenate.c \
exec_check_exit_status.c \
signals.c \
parsing_check_arg.c \
read_input.c \
builtin_cd.c \
builtin_cd_2.c \
builtin_echo.c \
builtin_export.c


# parsing_pid.c \

# cd_test2.c

SRC = ${FILE:.c=.o}

all : ${NAME}

${NAME}: ${SRC}
	make -C gnl
	${CC} ${CFLAGS} -o $@ $^ ${FLAGS} -Lgnl -lgnl

# test: test.c
# 	${CC} ${CFLAGS} -o $@ $^ ${FLAGS}

%.o:%.c
	${CC} ${CFLAGS} -c $^

clean: 
	make -C gnl clean
	rm -f parsing_test.o \
	pipex_path.o \
	parsing_dollar.o \
	minishell_utils.o \
	ft_strjoin.o \
	ft_split.o \
	ft_itoa.o \
	parsing_lst_utils.o \
	parsing_free.o \
	parsing_double_tab.o \
	parsing_triple_tab.o \
	exec_pipe.o \
	parsing_parsing_0.o \
	parsing_parsing_1.o \
	parsing_parsing_2.o \
	parsing_parsing_3.o \
	parsing_redir.o \
	parsing_lst_utils_2.o \
	parsing_init.o \
	cd_test_4.o \
	cd_test.o \
	parsing_concatenate.o \
	exec_check_exit_status.o \
	signals.o \
	parsing_check_arg.o \
	read_input.o \
	builtin_cd.o \
	builtin_cd_2.o \
	builtin_echo.o \
	builtin_export.o



fclean:clean
	make -C gnl fclean
	rm -f minishell

re:fclean
	make