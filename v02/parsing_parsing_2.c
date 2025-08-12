/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_parsing_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:37:57 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:37:59 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_parse_double_quote(char *str, t_list **shell, int *i, t_all *all);
int ft_parse_singlequote(char *str, t_list **shell, int *i, t_all *all);
char *ft_remove_quote(char *str);
int ft_parse_space(char *str, t_list **shell, int *i, t_all *all);

char *ft_remove_quote(char *str)
{
	int i;
	int j;
	char *new;
	int insinglequote;
	int indoublequote;

	i = 0;
	j = 0;
	insinglequote = 0;
	indoublequote = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while (str[i])
	{
		if (str[i] == '"' && !insinglequote)
		{
			indoublequote = !indoublequote;
			// j++;
		}
		else if (str[i] == '\'' && !indoublequote)
		{
			insinglequote = !insinglequote;
			// j++;
		}
		else if (insinglequote == 1 && str[i] == '"')
		{
			new[j] = str[i];
			j++;
		}
		else if (indoublequote == 1 && str[i] == '\'')
		{
			new[j] = str[i];
			j++;
		}
		else
		{
			new[j] = str[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	return (new);
}

int ft_parse_double_quote(char *str, t_list **shell, int *i, t_all *all)
{
	(void)all;
	int j;
	char *temp;
	int insinglequote;
	int indoublequote;

	insinglequote = 0;
	indoublequote = 0;
	j = 0;
	// while (str[*i + j] != '"' && str[*i + j] != '\0')
		// j++;

	while ((str[*i + j] != ' ' /*&& str[*i + j] != '"' && str[*i + j] != '\''*/
			&& str[*i + j] != '|' && str[*i + j] != '\0')
		|| (indoublequote == 1 || insinglequote == 1))
	{
		// Check if we reached end of string while still in quotes
		if (str[*i + j] == '\0' && (indoublequote == 1 || insinglequote == 1))
		{
			ft_err("minishell: syntax error: unclosed quote\n", NULL);
			return (-1);
		}
		if (str[*i + j] == '\\' && indoublequote == 0 && insinglequote == 0)
		{
			j++;
		}
		else if (str[*i +j] == '"' && !insinglequote)
		{
			indoublequote = !indoublequote;
			// if (indoublequote)
			// j++;
		}
		else if (str[*i +j] == '\'' && !indoublequote)
		{
			insinglequote = !insinglequote;
			// j++;
		}
		j++;
	}
	if (j > 0)
	{
		temp = ft_substr(str, *i, j);
// printf("TEMPdouble:%s\n", temp);
		char *temp3 = replace_dollar_test2(temp, all->env, all);
		// char *temp2 = ft_remove_quote(temp3);
		// printf("1:%s\n2:%s\n3:%s\n", temp, temp3, temp2);
		// tab = ft_add_double_tab(temp, tab);
		ft_add(shell, temp3, DOUBLEQUOTE);
		// printf("double:%s i:%d j:%d\n", temp, i, j);
		free(temp);
		// free(temp2);
	}
	*i = *i + j;
	if (str[*i] == '"')
		(*i)++;
	return (0);
}

int ft_parse_singlequote(char *str, t_list **shell, int *i, t_all *all)
{
	(void)all;
	int j;
	char *temp;
	int insinglequote;
	int indoublequote;

	insinglequote = 0;
	indoublequote = 0;
	j = 0;
	// while (str[*i + j] != '\'' && str[*i + j] != '\0')
		// j++;

	while ((str[*i + j] != ' ' /*&& str[*i + j] != '"' && str[*i + j] != '\''*/
			&& str[*i + j] != '|' && str[*i + j] != '\0')
		|| (indoublequote == 1 || insinglequote == 1))
	{
		if (str[*i + j] == '\0' && (indoublequote == 1 || insinglequote == 1))
		{
			ft_err("minishell: syntax error: unclosed quote\n", NULL);
			return (-1);
		}
		if (str[*i + j] == '\\' && indoublequote == 0 && insinglequote == 0)
		{
			j++;
		}
		else if (str[*i +j] == '"' && !insinglequote)
		{
			indoublequote = !indoublequote;
			// if (indoublequote)
			// j++;
		}
		else if (str[*i +j] == '\'' && !indoublequote)
		{
			insinglequote = !insinglequote;
			// j++;
		}
		j++;
	}


	if (j > 0)
	{
		temp = ft_substr(str, *i, j);
// printf("TEMPsingle:%s\n", temp);
		char *temp3 = replace_dollar_test2(temp, all->env, all);
		// char *temp2 = ft_remove_quote(temp3);
		// printf("1:%s\n2:%s\n3:%s\n", temp, temp3, temp2);
		// tab = ft_add_double_tab(temp, tab);
		ft_add(shell, temp3, SINGLEQUOTE);
		// printf("single:%s i:%d j:%d\n", temp, i, j);
		free(temp);
		// free(temp2);
	}
	*i = *i + j;
	if (str[*i] == '\'')
		(*i)++;
	j = 0;
	return (0);
}

int ft_parse_space(char *str, t_list **shell, int *i, t_all *all)
{
	(void)all;
	int j;
	int state;
	char *temp;
	int insinglequote;
	int indoublequote;

	insinglequote = 0;
	indoublequote = 0;
	j = 0;
	state = NORMAL;
	while ((str[*i + j] != ' ' /*&& str[*i + j] != '"' && str[*i + j] != '\''*/
			&& str[*i + j] != '|' && str[*i + j] != '\0')
		|| (indoublequote == 1 || insinglequote == 1))
	{
		if (str[*i + j] == '\0' && (indoublequote == 1 || insinglequote == 1))
		{
			ft_err("minishell: syntax error: unclosed quote\n", NULL);
			return (-1);
		}
		if (str[*i + j] == '\\' && indoublequote == 0 && insinglequote == 0)
		{
			j++;
		}
		else if (str[*i +j] == '"' && !insinglequote)
		{
			indoublequote = !indoublequote;
			// if (indoublequote)
			// j++;
		}
		else if (str[*i +j] == '\'' && !indoublequote)
		{
			insinglequote = !insinglequote;
			// j++;
		}
		j++;
	}
	if (j > 0)
	{
		temp = ft_substr(str, *i, j);
// printf("TEMPTEST:%s\n", temp);
		char *temp3 = replace_dollar_test2(temp, all->env, all);
		// char *temp2 = ft_remove_quote(temp3);
		// printf("1:%s\n2:%s\n3:%s\n", temp, temp3, temp2);
		// tab = ft_add_double_tab(temp, tab);

		ft_add(shell, temp3, state);
		// printf("space:%s i:%d j:%d\n", temp, i, j);
		free(temp);
		// free(temp2);
	}
	*i = *i + j;
	j = 0;
	return (0);
}