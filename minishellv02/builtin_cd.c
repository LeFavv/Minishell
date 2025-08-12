/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:23:58 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/12 17:29:26 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cd(char *str);
int	is_home(char *str);
int	cd_home(t_all **all);
int	cd_oldpwd(t_all **all);
int	cd_root(t_all **all);

int	is_cd(char *str)
{
	if (ft_strcmp(str, "cd") == 0)
		return (1);
	return (0);
}

int	is_home(char *str)
{
	if (ft_strcmp(str, "~") == 0)
		return (1);
	return (0);
}

int	cd_home(t_all **all)
{
	char	*path;
	char	cwd[PATH_MAX];

	path = get_env_var("HOME", (*all)->env);
	if (!path)
	{
		perror("minishell: cd: HOME not set\n");
		return (1);
	}
	getcwd(cwd, sizeof(cwd));
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	(*all)->env = ft_replace_double_tab("OLDPWD", cwd, (*all)->env);
	getcwd(cwd, sizeof(cwd));
	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
	return (0);
}

int	cd_oldpwd(t_all **all)
{
	char	*oldpwd;
	char	cwd[PATH_MAX];

	oldpwd = get_env_var("OLDPWD", (*all)->env);
	if (!oldpwd)
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
	getcwd(cwd, sizeof(cwd));
	if (chdir(oldpwd) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	(*all)->env = ft_replace_double_tab("OLDPWD", cwd, (*all)->env);
	getcwd(cwd, sizeof(cwd));
	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
	ft_putstr(cwd);
	write(1, "\n", 1);
	return (0);
}

int	cd_root(t_all **all)
{
	char	cwd[PATH_MAX];

	if (chdir("/"))
		return (perror("errno"), -1);
	getcwd("/", 1);
	getcwd(cwd, sizeof(cwd));
	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
	return (0);
}
