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

	while (*lst != NULL)
	{
		del = *lst;
		*lst = (*lst)->next;
		free(del->str);
		free(del);
	}
	*lst = NULL;
}

void ft_free_all(t_all *all)
{
	if (all->str)
		free(all->str);
	if (all->shell)
		ft_clear(&all->shell);
	int j = 0;
	if (all->t_cmd && all->t_cmd->cmd_tab)
	{
		while (j < all->t_cmd->nbr_cmd && all->t_cmd->cmd_tab[j].cmd_args)
		{
			ft_free_double_tab(all->t_cmd->cmd_tab[j].cmd_args);
			j++;
		}
		free(all->t_cmd->cmd_tab);
	}
	if (all->t_cmd)
		free(all->t_cmd);
}