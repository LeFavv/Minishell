/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:17:21 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/06 12:17:22 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char **env)
{
	char	*str;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
		{
			str = env[i];
			str = str + 5;
			return (str);
		}
		i++;
	}
	return (NULL);
}

char	*check_path(char **tab, char **cmd)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = 0;
	while (tab[i])
	{
		temp = ft_strjoin(tab[i], "/");
		if (!temp)
			return (NULL);
		temp2 = ft_strjoin(temp, cmd[0]);
		if (!temp2)
			return (free(temp), NULL);
		free(temp);
		if (access(temp2, X_OK) == 0)
			return (temp2);
		free(temp2);
		i++;
	}
	return (NULL);
}

void	exec_v2(char *arg, char **env)
{
	char	*str;
	char	**envpath;
	char	**tab;
	char	*path;

	str = get_path(env);
	envpath = ft_split(str, ':');
	if (!envpath)
		return ;
	tab = ft_split(arg, ' ');
	if (!tab)
		return ;
		// return ft_free(envpath, NULL, NULL);
	path = check_path(envpath, tab);
	if (!path)
		return ;
		// return (ft_free(envpath, tab, NULL);
	// perror("perror1:");
	// if (execve(path, tab, env) == -1)
		// return ;
	execve(path, tab, env);
	// perror("perror2:");
		// return (ft_free(envpath, tab, path);
	ft_free(envpath, tab, path);
	return ;
}