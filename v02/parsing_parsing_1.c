/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_parsing_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:35:20 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:35:21 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_parse_decoupe(char *str, t_list **shell, t_all *all);

//Premiere eteape du parsing, separe en token en fonction des separateur '"', '\'', ' '
//prend en compte les echappement avec '\'
int ft_parse_decoupe(char *str, t_list **shell, t_all *all)
{
	int i = 0;
	int result = 0;
	
	while (str[i])
	{
		if (str[i] == ' ')
			i++;
		else if (str[i] == '"')
		{
			result = ft_parse_double_quote(str, shell, &i, all);
			if (result == -1)
				return (-1);
		}
		else if (str[i] == '\'')
		{
			result = ft_parse_singlequote(str, shell, &i, all);
			if (result == -1)
				return (-1);
		}
		else if (str[i] == '|')
		{
			ft_parse_pipe(str, shell, &i);
		}
		else if (str[i] == '>')
		{
			ft_parse_out(str, shell, &i);
		}
		else if (str[i] == '<')
		{
			ft_parse_in(str, shell, &i);
		}
		else
		{
			result = ft_parse_space(str, shell, &i, all);
			if (result == -1)
				return (-1);
		}
	}
	return (0);
}
