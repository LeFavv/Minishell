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

int	ft_open_pipe(t_commande *t_cmd, t_all **all)
{
	int	i;

	if (!t_cmd || !t_cmd->cmd_tab || !all || !*all)
		return (1);

	i = 0;
	while (i < t_cmd->nbr_cmd - 1)
	{
		// Initialiser les descripteurs à -1 pour indiquer qu'ils ne sont pas ouverts
		t_cmd->cmd_tab[i].fd[0] = -1;
		t_cmd->cmd_tab[i].fd[1] = -1;
		
		if (pipe(t_cmd->cmd_tab[i].fd) == -1)
		{
			// En cas d'erreur, fermer tous les pipes déjà ouverts
			while (--i >= 0)
			{
				close(t_cmd->cmd_tab[i].fd[0]);
				close(t_cmd->cmd_tab[i].fd[1]);
				t_cmd->cmd_tab[i].fd[0] = -1;
				t_cmd->cmd_tab[i].fd[1] = -1;
			}
			// Libérer les ressources allouées
			// if ((*all)->str)
			// 	free((*all)->str);
			// if ((*all)->shell)
			// 	ft_clear(&(*all)->shell);
			return (1);
		}
		i++;
	}
	return (0);
}

void	ft_close_pipe(t_commande *t_cmd)
{
	int	i;

	if (!t_cmd || !t_cmd->cmd_tab)
		return;
		
	i = 0;
	while (i < t_cmd->nbr_cmd - 1)
	{
		if (t_cmd->cmd_tab[i].fd[0] >= 0)
		{
			close(t_cmd->cmd_tab[i].fd[0]);
			t_cmd->cmd_tab[i].fd[0] = -1;
		}
		if (t_cmd->cmd_tab[i].fd[1] >= 0)
		{
			close(t_cmd->cmd_tab[i].fd[1]);
			t_cmd->cmd_tab[i].fd[1] = -1;
		}
		i++;
	}
}

// Vérification des arguments d'entrée
// Fermer les descripteurs de fichier d'entrée s'ils sont valides
// Fermer les descripteurs de fichier de sortie s'ils sont valides
// Marquer comme fermé
int	ft_close_fd(t_all **all)
{
	int	i;

	if (!all || !*all || !(*all)->t_cmd || !(*all)->t_cmd->cmd_tab)
		return (-1);
	i = 0;
	while (i < (*all)->t_cmd->nbr_cmd)
	{
		if ((*all)->t_cmd->cmd_tab[i].infd >= 0)
		{
			close((*all)->t_cmd->cmd_tab[i].infd);
			(*all)->t_cmd->cmd_tab[i].infd = -1;
		}
		if ((*all)->t_cmd->cmd_tab[i].outfd >= 0)
		{
			close((*all)->t_cmd->cmd_tab[i].outfd);
			(*all)->t_cmd->cmd_tab[i].outfd = -1;
		}
		i++;
	}
	return (0);
}

// Seulement attendre les processus valides
void	ft_waitpid(t_commande *t_cmd)
{
	int	j;

	j = 0;
	while (j < t_cmd->nbr_cmd)
	{
		if (t_cmd->cmd_tab[j].id1 > 0)
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
