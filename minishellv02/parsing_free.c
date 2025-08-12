/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:28:46 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:28:46 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_double_tab(char **tab);
void	ft_clear(t_list **lst);

void	ft_free_double_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	ft_clear(t_list **lst)
{
	t_list	*del;
	t_list	*next;

	if (!lst || !*lst)
		return;

	del = *lst;
	while (del != NULL)
	{
		next = del->next;  // Sauvegarde le pointeur vers le nœud suivant
		
		if (del->str)      // Vérification pour éviter de libérer un pointeur NULL
		{
			free(del->str);
			del->str = NULL;
		}
		free(del);
		del = next;        // Avance au nœud suivant
	}
	*lst = NULL;  // Marque la liste comme vide
}

void ft_free_all(t_all *all)
{
	int j;

	if (!all)
		return;
	ft_close_fd(&all);
	if (all->str)
	{
		free(all->str);
		all->str = NULL;
	}
	if (all->shell)
		ft_clear(&all->shell);
	j = 0;
	if (all->t_cmd && all->t_cmd->cmd_tab)
	{
		while (j < all->t_cmd->nbr_cmd/* && all->t_cmd->cmd_tab[j].cmd_args*/)
		{
			if (all->t_cmd->cmd_tab[j].cmd_args)
			{
				ft_free_double_tab(all->t_cmd->cmd_tab[j].cmd_args);
				all->t_cmd->cmd_tab[j].cmd_args = NULL;
			}
			j++;
		}
		free(all->t_cmd->cmd_tab);
		all->t_cmd->cmd_tab = NULL;
	}
	if (all->t_cmd)
	{
		free(all->t_cmd);
		all->t_cmd = NULL;
	}
	// // Libération de la chaîne d'état de sortie si existante
	// if (all->exit_status_char)
	// {
	// 	free(all->exit_status_char);
	// 	all->exit_status_char = NULL;
	// }
}
