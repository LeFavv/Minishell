/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_parsing_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:38:02 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:38:03 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_parse_pipe(char *str, t_list **shell, int *i);
int ft_parse_out(char *str, t_list **shell, int *i);
int ft_parse_in(char *str, t_list **shell, int *i);

int ft_parse_pipe(char *str, t_list **shell, int *i)
{
	int j;
	char *temp;

	j = 0;
	j++;
	while (str[*i + j] == '|' && str[*i + j] != '\0')
		j++;
	if (j > 0)
	{
		temp = ft_substr(str, *i, j);
		// tab = ft_add_double_tab(temp, tab);
		if (ft_strlen(temp) == 1)
			ft_add(shell, temp, PIPE);
		else
			ft_add(shell, temp, ERROR);
		// printf("single:%s i:%d j:%d\n", temp, i, j);
		free(temp);
	}
	*i = *i + j;
	if (str[*i] == '|')
		(*i)++;
	j = 0;
	return (0);
}

int ft_parse_out(char *str, t_list **shell, int *i)
{
	int j;
	char *temp;

	j = 0;
	j++;
	while (str[*i + j] == '>' && str[*i + j] != '\0')
		j++;
	if (j > 0)
	{
		temp = ft_substr(str, *i, j);
		// tab = ft_add_double_tab(temp, tab);
		if (ft_strlen(temp) == 1)
			ft_add(shell, temp, OUTPUT);
		else if (ft_strlen(temp) == 2)
			ft_add(shell, temp, APPEND);
		else
			ft_add(shell, temp, ERROR);
		// printf("single:%s i:%d j:%d\n", temp, i, j);
		free(temp);
	}
	*i = *i + j;
	if (str[*i] == '>')
		(*i)++;
	j = 0;
	return (0);
}

int ft_parse_in(char *str, t_list **shell, int *i)
{
	int j;
	char *temp;

	j = 0;
	j++;
	while (str[*i + j] == '<' && str[*i + j] != '\0')
		j++;
	if (j > 0)
	{
		temp = ft_substr(str, *i, j);
		// tab = ft_add_double_tab(temp, tab);
		if (ft_strlen(temp) == 1)
			ft_add(shell, temp, INPUT);
		else if (ft_strlen(temp) == 2)
			ft_add(shell, temp, HEREDOC);
		else
			ft_add(shell, temp, ERROR);
		// printf("single:%s i:%d j:%d\n", temp, i, j);
		free(temp);
	}
	*i = *i + j;
	if (str[*i] == '<')
		(*i)++;
	j = 0;
	return (0);
}