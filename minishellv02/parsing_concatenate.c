/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_concatenate.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:59:54 by kevwang           #+#    #+#             */
/*   Updated: 2025/08/11 12:59:55 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_concatenate(t_list **lst)
{
	// t_list *current;
	// t_list *next;
	t_list *temp;
	t_list *current;
	// char *new_str;

	if (!lst || !*lst)
		return;
	// current = *lst;
	temp = *lst;
	// if ((*lst)->state == NORMAL && (!(*lst)->str || (*lst)->str[0] == '\0'))
	// {
	// 	(*lst) = (*lst)->next;
	// }
	while (*lst && (*lst)->next)
	{
		if ((*lst)->next->state == NORMAL && (!(*lst)->next->str || (*lst)->next->str[0] == '\0'))
		{
			current = (*lst)->next;
			(*lst)->next = (*lst)->next->next;
			free(current->str);
			free(current);
			continue ;
		}
		(*lst) = (*lst)->next;
	}
	(*lst) = temp;
	if ((*lst)->state == NORMAL && (!(*lst)->str || (*lst)->str[0] == '\0'))
	{
		(*lst) = (*lst)->next;
	}
	// (*lst) = temp;
}