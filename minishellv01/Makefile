# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/26 12:48:55 by kevwang           #+#    #+#              #
#    Updated: 2025/06/26 12:48:57 by kevwang          ###   ########.fr        #
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
parsing_parsing_1.c \
parsing_parsing_2.c \
parsing_parsing_3.c \
parsing_redir.c \
parsing_lst_utils_2.c \
parsing_init.c \
cd_test_4.c \
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
	parsing_parsing_1.o \
	parsing_parsing_2.o \
	parsing_parsing_3.o \
	parsing_redir.o \
	parsing_lst_utils_2.o \
	parsing_init.o \
	cd_test2.o \
	cd_test.o \

fclean:clean
	rm -f minishell

re:fclean
	make