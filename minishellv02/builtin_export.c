/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:54:08 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/12 18:34:32 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_export(char *str);
int		is_alpha(char *str);
void	if_equal_pos(t_all **all, char *var_name, int i, char **tab, char *equal_pos);
int		if_equal_pos_return(char *var_name, char **tab);
int		ft_export(char **tab, t_all **all);

int	is_export(char *str)
{
	if (ft_strcmp(str, "export") == 0)
		return (1);
	return (0);
}

int	is_alpha(char *str)
{
	int  i;
	
	i = 0;
	if (!(str[i] >='a' && str[i] <= 'z') 
		&& !(str[i] >= 'A' && str[i] <= 'Z') && str[i] != '_')
		return (0);
	if (ft_strlen(str) == 1)
		return (1);
	i++;
	while (i < ((int)ft_strlen(str) - 1))
	{
		if (!((str[i] >='a' && str[i] <= 'z' ) || (str[i] >= 'A' && str[i] <= 'Z')))
		{
			if (!(str[i] >= '0' && str[i] <= '9'))
			{
				if (str[i] != '_')
					return (0);
			}
		}
		i++;
	}
	if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' 
		&& str[i] <= 'Z') && str[i] != '_')
		return (0);
	return (1);
}

void if_equal_pos(t_all **all, char *var_name, int i, char **tab, char *equal_pos)
{
	if (!get_env_var(var_name, (*all)->env))
		(*all)->env = ft_add_double_tab(tab[i], (*all)->env);
	else
		(*all)->env = ft_replace_double_tab(var_name, equal_pos + 1, (*all)->env);   
}

int if_equal_pos_return(char *var_name, char **tab)
{
	if (!var_name)
		return (ft_err(tab[0], "malloc failed"), 1);
	if (is_alpha(var_name) == 0|| !var_name)
		return (ft_err(tab[0], "not a valid identifier"), free(var_name), 1);
	return (0);
}

int	ft_export(char **tab, t_all **all)
{
	int i;
	char *var_name;
	char *equal_pos;

	i= 1;
	while (tab[i])
	{
		equal_pos = ft_strchr(tab[i], '=');
		if ((!equal_pos && !is_alpha(tab[i])) || tab[i][0] == '=')
			return (ft_err(tab[0], "not a valid identifier"), 1);
		if (ft_strlen(tab[i]) == 1 && tab[i][0] == '=')
			return (ft_err(tab[0], "not a valid identifier"), 1);
		else if (!equal_pos && is_alpha(tab[i]))
			return (0);
		if (equal_pos)
		{
			var_name = ft_substr(tab[i], 0, equal_pos - tab[i]);
			if (!var_name || (is_alpha(var_name) == 0|| !var_name))
				return (if_equal_pos_return(var_name, tab));
			if_equal_pos(all, var_name, i, tab, equal_pos);
			free(var_name);
		}
		i++;
	}
	return (0);
}
