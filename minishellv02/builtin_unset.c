/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 06:53:08 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/14 07:02:55 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(char **tab, t_all **all);
int	ft_env(t_all **all);
int	is_pwd(char *str);
int	ft_pwd(t_all **all);

int	ft_unset(char **tab, t_all **all)
{
	int	i;

	i = 1;
	while (tab[i])
	{
		if (get_env_var(tab[i], (*all)->env))
			(*all)->env = ft_remove_double_tab(tab[i], (*all)->env);
		i++;
	}
	return (0);
}

int	ft_env(t_all **all)
{
	int	i;

	i = 1;
	while ((*all)->env[i])
	{
		ft_putstr((*all)->env[i]);
		write (1, "\n", 1);
		i++;
	}
	return (0);
}

int	is_pwd(char *str)
{
	if (ft_strcmp(str, "pwd") == 0)
		return (1);
	return (0);
}

int	ft_pwd(t_all **all)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		return (printf("%s\n", cwd), 0);
	}
	else
	{
		perror("pwd: error retrieving current directory:");
		perror(" getcwd: cannot access parent directories: ");
		(*all)->exit_status = 1;
	}
	return (1);
}
