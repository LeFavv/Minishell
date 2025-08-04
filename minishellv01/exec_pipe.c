/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:33:33 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:33:36 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_open_pipe(t_commande *t_cmd)
{
	int	i;

	i = 0;
	while (i < t_cmd->nbr_cmd - 1)
	{
		if (pipe(t_cmd->cmd_tab[i].fd) == -1)
			return (1);
		i++;
	}
	return (0);
}

void	ft_close_pipe(t_commande *t_cmd)
{
	int	i;

	i = 0;
	while (i < t_cmd->nbr_cmd - 1)
	{
		close(t_cmd->cmd_tab[i].fd[0]);
		close(t_cmd->cmd_tab[i].fd[1]);
		i++;
	}
}

void	ft_waitpid(t_commande *t_cmd)
{
	int	j;

	j = 0;
	while (j < t_cmd->nbr_cmd)
	{
		waitpid(t_cmd->cmd_tab[j].id1, &t_cmd->status, 0);
		j++;
	}
}

int	ft_dup(int fd0, int fd1)
{
	if (dup2(fd0, 0) == -1)
		return (-1);
	if (dup2(fd1, 1) == -1)
		return (-1);
	return (0);
}

