/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:37:19 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/05 16:37:20 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(char **tab1, char **tab2, char *str)
{
	if (tab1)
		ft_free_double_tab(tab1);
	if (tab2)
		ft_free_double_tab(tab2);
	if (str)
		free(str);
}

void	ft_err(char *msg1, char *msg2)
{
	write(2, msg1, ft_strlen(msg1));
	if (msg2)
	{
		write(2, ": ", 2);
		write(2, msg2, ft_strlen(msg2));
		write(2, "\n", 1);
	}
}

char	*get_path(char **env)
{
	char	*str;
	int		i;

	i = 0;
	if (!env)
		return (NULL);
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
	if (!tab || !cmd || !cmd[0] || !cmd[0][0])
		return (NULL);
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
/*
char	*ft_str_last(char *str)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (str[i])
		i++;
	while (str[i - j] != '/' && j < i)
		j++;
	if (str[i - j] == '/')
		j--;
	return (&str[i - j]);
}
*/

int	exec2(char **tab, char **env)
{
	char	*str;
	char	**envpath;
	char	*path;

	if (!tab || !tab[0] || !tab[0][0])
		return (ft_free_double_tab(tab), -1);
	str = get_path(env);
	if (str)
	{
		envpath = ft_split(str, ':');
		if (!envpath)
			return (ft_free_double_tab(tab), -1);
		path = check_path(envpath, tab);
		if (!path)
			return (ft_err(tab[0], "command not found"),
				ft_free(envpath, tab, NULL), -1);
		if (execve(path, tab, env) == -1)
			return (ft_err(tab[0], "command not found"),
				ft_free(envpath, tab, path), -1);
		return (ft_free(envpath, tab, path), 0);
	}
	return (0);
}

int	exec(char **tab, char **env)
{
	// char	**tab;

	// tab = ft_split(arg, ' ');
	// if (!tab)
		// return (-1);
	int stats;
	struct stat buf;

	if (!tab || !tab[0] || !tab[0][0])
		return (ft_free_double_tab(tab), -1);
	if (tab[0][0] == '/' || (tab[0][0] == '.' && tab[0][1] == '/'))
	{
		stats = stat(tab[0], &buf);
		if (stats == 0 && S_ISDIR(buf.st_mode))
			return (ft_err(tab[0], "Is a directory"),  exit(126), -1);
		if (access(tab[0], F_OK) == 0 && access(tab[0], X_OK) == -1)
			return (ft_err(tab[0], "Permission denied"),  exit(126), -1);
		if (access(tab[0], X_OK) == -1)
			return (ft_err(tab[0], "No such file or directory"), 
				/*ft_free_double_tab(tab),*/ /*exit(126), */-1);
		if (execve(tab[0], tab, env) == -1)
			return (ft_err(tab[0], "No such file or directory"), exit(126), /*ft_free_double_tab(tab),*/ -1);
		return (/*ft_free_double_tab(tab), */0);
	}
	if (exec2(tab, env) == -1)
		return (-1);
	return (/*ft_free_double_tab(tab), */0);
}

//test
//caca pipex_path.c ft_split.c pipex_free.c ft_strjoin.c pipex_utlis.c 
// int	main(int argc, char **argv, char **env)
// {
// 	// (void)env;
// 	if (argc == 2)
// 	{
// 	// 	char **tab = ft_split(argv[1], ' ');
// 	// 	int i = 0;
// 	// 	while (tab[i])
// 	// 	{
// 	// 		printf("%s\n", tab[i]);
// 	// 		i++;
// 	// 	}
// 		exec(argv[1], env);
// 		// free(tab);
// 	}
// 	return (0);
// }
