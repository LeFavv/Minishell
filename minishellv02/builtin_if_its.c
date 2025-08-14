/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_if_its.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 07:58:26 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/14 08:04:20 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin_2(char **tab, t_all **all);
int	is_builtin_3(char **tab, t_all **all);

int	is_builtin_2(char **tab, t_all **all)
{
	int	exit_code;

	if (is_export(tab[0]))
	{
		exit_code = ft_export(tab, all);
		(*all)->exit_status = exit_code;
		return (1);
	}
	if (ft_strcmp(tab[0], "unset") == 0)
	{
		exit_code = ft_unset(tab, all);
		(*all)->exit_status = exit_code;
		return (1);
	}
	if (ft_strcmp(tab[0], "env") == 0 && !tab[1])
	{
		ft_env(all);
		return (1);
	}
	if (ft_strcmp(tab[0], "exit") == 0)
	{
		ft_exit(tab, all);
		return (1);
	}
	return (0);
}

int	is_builtin_3(char **tab, t_all **all)
{
	int	exit_code;

	if (is_cd(tab[0]))
	{
		exit_code = homemade_cd(tab, all);
		(*all)->exit_status = exit_code;
		return (1);
	}
	if (is_echo(tab[0]))
	{
		exit_code = ft_echo(tab);
		(*all)->exit_status = exit_code;
		return (exit_code);
	}
	if (is_pwd(tab[0]))
	{
		ft_pwd(all);
		return (1);
	}
	if (is_builtin_2(tab, all))
		return (1);
	return (0);
}
