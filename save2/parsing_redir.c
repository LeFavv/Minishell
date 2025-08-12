/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:43:27 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:43:28 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_create_fd(t_list **shell, t_redir **t_red);

//creation des fd a partir des redirections
//pas encore correct
int ft_create_fd(t_list **shell, t_redir **t_red)
{
	t_list *temp;

	temp = *shell;
	while (*shell)
	{
		// printf("fd state%d\n", shell->state);
		if ((*shell)->state == INFILE)
		{
			(*t_red)->infd = open((*shell)->str, O_RDONLY, 0644);
			if ((*t_red)->infd < 0)
				perror("infd error");
		}
		if ((*shell)->state == OUTFILE)
			(*t_red)->outfd = open((*shell)->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if ((*shell)->state == OUTFILEAPPEND)
			(*t_red)->outfd = open((*shell)->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
		// printf("fd:%d\n", (*t_red)->infd);
		(*shell) = (*shell)->next;
	}
	(*shell) = temp;
	// printf("infd:%d outfd:%d\n", (*t_red)->infd, (*t_red)->outfd);
	return (0);
}
