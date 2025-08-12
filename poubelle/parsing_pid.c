/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:03:52 by marvin            #+#    #+#             */
/*   Updated: 2025/08/07 16:03:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_pipe_pid(t_all *all)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		close(all->t_pid->cmd[i].fd[0]);
		close(all->t_pid->cmd[i].fd[1]);
		i++;
	}
}

int	ft_redir(t_all *all, int i)
{
	// if (p->nbr_cmd == 1)
	// {
	// 	if (ft_dup(p->infd, p->outfd) == -1)
	// 		return (ft_close_pipe(p), free(p->pipefd), free(p), 1);
	// }
	if (i == 0)
	{
		dup2(all->t_pid->cmd[i].fd[1], 1);
		// if (ft_dup(p->infd, p->pipefd[i].fd[1]) == -1)
			// return (ft_close_pipe(p), free(p->pipefd), free(p), 1);
	}
	else if (i == 2)
	{
		all->t_pid->outfd = open("temp2", O_WRONLY | O_TRUNC | O_CREAT, 0644);
		// if (p->outfd < 0)
			// ft_close_all(p, EXIT);
		dup2(all->t_pid->outfd, 1);
		dup2(all->t_pid->cmd[i - 1].fd[0], 0);
		// if (ft_dup(p->pipefd[i - 1].fd[0], p->outfd) == -1)
		// 	return (ft_close_pipe(p), free(p->pipefd), free(p), 1);
	}
	else
	{
		dup2(all->t_pid->cmd[i].fd[1], 1);
		dup2(all->t_pid->cmd[i - 1].fd[0], 0);
	}
		// if (ft_dup(p->pipefd[i - 1].fd[0], p->pipefd[i].fd[1]) == -1)
		// 	return (ft_close_pipe(p), free(p->pipefd), free(p), 1);
	return (0);
}

char *ft_get_pid(t_all *all)
{
	all->t_pid = malloc(sizeof(t_pid));
	if (!all->t_pid)
		return (NULL);
	all->t_pid->cmd = malloc(sizeof(t_pid_cmd) * 3);
	if (!all->t_pid->cmd)
		return (NULL);
	char *str = malloc(50);
	if (!str)
		return (NULL);
	int readfd;
	all->t_pid->cmd[0].tab[0] = "cat";
	all->t_pid->cmd[0].tab[1] = "/proc/self/status";
	all->t_pid->cmd[0].tab[2] = NULL;

	all->t_pid->cmd[1].tab[0] = "grep";
	all->t_pid->cmd[1].tab[1] = "PPid";
	all->t_pid->cmd[1].tab[2] = NULL;

	all->t_pid->cmd[2].tab[0] = "awk";
	all->t_pid->cmd[2].tab[1] = "{print $2}";
	all->t_pid->cmd[2].tab[2] = NULL;

	int	i;

	i = 0;
	while (i < 3)
	{
		if (pipe(all->t_pid->cmd[i].fd) == -1)
			return (NULL);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		all->t_pid->cmd[i].id1 = fork();
		if (all->t_pid->cmd[i].id1 == 0)
		{
			ft_redir(all, i);
			ft_close_pipe_pid(all);
			// if (all->t_pid->outfd >= 0)
				// close(all->t_pid->outfd);
			if (exec(all->t_pid->cmd[i].tab, all->env) == -1)
			{
				perror("command not found");
				// ft_close_all(pipex, 127);
			}
		}
		i++;
	}
	ft_close_pipe_pid(all);
	
	// Attendre que tous les processus se terminent
	int j = 0;
	while (j < 3)
	{
		waitpid(all->t_pid->cmd[j].id1, NULL, 0);
		j++;
	}
	
	// Lire le résultat depuis le fichier temporaire
	int temp_fd = open("temp2", O_RDONLY);
	if (temp_fd < 0)
		return (NULL);
	readfd = read(temp_fd, str, 49);
	close(temp_fd);
	if (readfd > 0)
		str[readfd - 1] = '\0';
	else
		str[0] = '\0';
	free(all->t_pid->cmd);
	free(all->t_pid);
	// Supprimer le fichier temporaire
	unlink("temp2");
	return (str);
}
