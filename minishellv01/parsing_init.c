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