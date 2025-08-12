/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:48:57 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:48:58 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_count_commands(t_list *lst);

int ft_count_commands(t_list *lst)
{
	int count;

	count = 1;
	while (lst)
	{
		if (lst->state == PIPE)
			count++;
		lst = lst->next;
	}
	return (count);
}

int ft_init_triple_tab(t_all **all)
{
	(*all)->t_cmd = malloc(sizeof(t_commande));
	if (!(*all)->t_cmd)
		return (-2);
	(*all)->t_cmd->nbr_cmd = ft_count_commands((*all)->shell);
	(*all)->t_cmd->cmd_tab = malloc(sizeof(t_cmd_tab) * (*all)->t_cmd->nbr_cmd);
	if (!(*all)->t_cmd->cmd_tab)
	{
		free((*all)->t_cmd);
		(*all)->t_cmd = NULL;
		return (-2);
	}
	ft_set_triple_tab_null((*all)->t_cmd);
	return (0);
}
