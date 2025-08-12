/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lst_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:24:31 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:24:32 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_lst(char *str, int state);
void	ft_lstadd_back(t_list **lst, t_list *new);
int  ft_add(t_list **lst, char *str, int state);

t_list	*ft_lst(char *str, int state)
{
	t_list	*newlist;

	// if (!str || !str[0])
		// return (NULL);

	newlist = malloc(sizeof(t_list));
	if (!newlist)
	{
		return (NULL);
	}
	newlist->str = ft_strdup(str);
	if (!newlist->str)
		return (free(newlist), NULL);
	newlist->state = state;
	newlist->next = NULL;
	return (newlist);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	temp = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while ((*lst)->next != NULL)
	{
		*lst = (*lst)->next;
	}
	(*lst)->next = new;
	*lst = temp;
}

int  ft_add(t_list **lst, char *str, int state)
{
	if (!(*lst))
	{
		(*lst) = ft_lst(str, state);
		if (!(*lst))
			return (-2);
		return (0);
	}
	t_list *temp = *lst;
	while ((*lst)->next)
	{
		(*lst) = (*lst)->next;
	}
	(*lst)->next = ft_lst(str, state);
	if (!(*lst)->next)
			return (-2);
	// (*lst)->next->state = state;
	(*lst) = temp;
	return (0);
}
