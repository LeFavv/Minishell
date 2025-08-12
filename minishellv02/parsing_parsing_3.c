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
		if (!temp)
			return (-2);
		// tab = ft_add_double_tab(temp, tab);
		if (ft_strlen(temp) == 1)
		{
			if (ft_add(shell, temp, PIPE) == -2)
				return (free(temp), -2);
		}
		else if (ft_strlen(temp) == 2)
			return (free(temp), ft_err("syntax error near unexpected token `||'\n", NULL), -1);
			// ft_add(shell, temp, ERROR);
		else 
			return (free(temp), ft_err("syntax error near unexpected token `||'\n", NULL), -1);
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
		if (!temp)
			return (-2);
		// tab = ft_add_double_tab(temp, tab);
		if (ft_strlen(temp) == 1)
		{
			if (ft_add(shell, temp, OUTPUT) == -2)
				return (free(temp), -2);
		}
		else if (ft_strlen(temp) == 2)
		{
			if (ft_add(shell, temp, APPEND) == -2)
				return (free(temp), -2);
		}
		else if (ft_strlen(temp) == 3)
		{
			return (free(temp), ft_err("syntax error near unexpected token `>'\n", NULL), -1);
		}
			// ft_add(shell, temp, ERROR);
		else 
			return (free(temp), ft_err("syntax error near unexpected token `>>'\n", NULL), -1);
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
		if (!temp)
			return (-2);
		// tab = ft_add_double_tab(temp, tab);
		if (ft_strlen(temp) == 1)
		{
			if (ft_add(shell, temp, INPUT) == -2)
				return (free(temp), -2);
		}
		else if (ft_strlen(temp) == 2)
		{
			if (ft_add(shell, temp, HEREDOC) == -2)
				return (free(temp), -2);
		}
		else if (ft_strlen(temp) == 3)
			return (free(temp), ft_err("syntax error near unexpected token `<'\n", NULL), -1);
			// ft_add(shell, temp, ERROR);
		else 
			return (free(temp), ft_err("syntax error near unexpected token `<<'\n", NULL), -1);
		// printf("single:%s i:%d j:%d\n", temp, i, j);
		free(temp);
	}
	*i = *i + j;
	if (str[*i] == '<')
		(*i)++;
	j = 0;
	return (0);
}