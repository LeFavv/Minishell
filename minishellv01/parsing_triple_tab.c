/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_triple_tab.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:31:45 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:31:45 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_print_triple_tab(t_commande *t_cmd);
int ft_set_triple_tab_null(t_commande *t_cmd);
int ft_create_triple_tab(t_list **shell ,t_commande **t_cmd, t_all **all);

int ft_print_triple_tab(t_commande *t_cmd)
{
	int j;

	j = 0;
	while (j < t_cmd->nbr_cmd)
	{
		if (t_cmd->cmd_tab[j].cmd_args)
		{
			printf("tab:%d\n", j);
			ft_print_tab(t_cmd->cmd_tab[j].cmd_args);
			printf("infd:%d\n", t_cmd->cmd_tab[j].infd);
			printf("outfd:%d\n", t_cmd->cmd_tab[j].outfd);
		}
		j++;
	}
	return (0);
}

//juste initialise a NULL les char**
int ft_set_triple_tab_null(t_commande *t_cmd)
{
	int i = 0;
	while (i < t_cmd->nbr_cmd)
	{
		t_cmd->cmd_tab[i].cmd_args = NULL;
		t_cmd->cmd_tab[i].input_failed = 0;
		t_cmd->cmd_tab[i].output_failed = 0;
		t_cmd->cmd_tab[i].id1 = -1;
		t_cmd->cmd_tab[i].infd = -1;
		t_cmd->cmd_tab[i].outfd = -1;
		t_cmd->cmd_tab[i].in_str = NULL;
		t_cmd->cmd_tab[i].out_str = NULL;
		i++;
	}
	return (0);
}
/*
int ft_create_triple_tab(t_list **shell ,t_commande **t_cmd, t_all **all)
{
	// t_list *temp = *shell;

	int i = 0;
	int prev_infd = -1;
    int prev_outfd = -1;
    (*t_cmd)->cmd_tab[i].infd = -1;
    (*t_cmd)->cmd_tab[i].outfd = -1;
    // (*t_cmd)->cmd_tab[i].errfd = -1;
	t_list *temp = *shell;
	while (*shell != NULL)
	{
		if ((*shell)->state == NORMAL || (*shell)->state == DOUBLEQUOTE || (*shell)->state == SINGLEQUOTE)
			(*t_cmd)->cmd_tab[i].cmd_args = ft_add_double_tab((*shell)->str, (*t_cmd)->cmd_tab[i].cmd_args);
		if ((*shell)->state == INFILE)
		{
			if (prev_infd != -1)
				close(prev_infd);
			(*t_cmd)->cmd_tab[i].infd = open((*shell)->str, O_RDONLY, 0644);
			if ((*t_cmd)->cmd_tab[i].infd < 0)
			{
				perror((*shell)->str);
				(*all)->exit_status = 1; // Set exit status to indicate error
				*shell = temp;
                return (-1);
			}
			prev_infd = (*t_cmd)->cmd_tab[i].infd;
		}
		if ((*shell)->state == OUTFILE)
		{
			if (prev_outfd != -1)
				close(prev_outfd);
			(*t_cmd)->cmd_tab[i].outfd = open((*shell)->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if ((*t_cmd)->cmd_tab[i].outfd < 0)
			{
				(*all)->exit_status = 1; // Set exit status to indicate error
				// perror((*shell)->str);
				*shell = temp;
                return (-1);
			}
			prev_outfd = (*t_cmd)->cmd_tab[i].outfd;
		}
		if ((*shell)->state == OUTFILEAPPEND)
	    {
	        if (prev_outfd != -1)
	            close(prev_outfd);
	        (*t_cmd)->cmd_tab[i].outfd = open((*shell)->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	        if ((*t_cmd)->cmd_tab[i].outfd < 0)
	        {
				(*all)->exit_status = 1; // Set exit status to indicate error
	            // perror((*shell)->str);
	            *shell = temp;
	            return (-1);
	        }
	        prev_outfd = (*t_cmd)->cmd_tab[i].outfd;
	    }
		if ((*shell)->state == PIPE)
		{
			i++;
			// Initialiser les champs pour la nouvelle commande
			(*t_cmd)->cmd_tab[i].infd = -1;
			(*t_cmd)->cmd_tab[i].outfd = -1;
			// Réinitialiser les variables de suivi des fd précédents
			prev_infd = -1;
			prev_outfd = -1;
		}
		(*shell) = (*shell)->next;
	}
	(*shell) = temp;
	return (0);
}
*/

int ft_create_triple_tab(t_list **shell ,t_commande **t_cmd, t_all **all)
{
	// t_list *temp = *shell;

	int i = 0;
	int prev_infd = -1;
    int prev_outfd = -1;

    // int input_fail = 0;
    // int output_fail = 0;
    
    (*t_cmd)->cmd_tab[i].infd = -1;
    (*t_cmd)->cmd_tab[i].outfd = -1;
    (*t_cmd)->cmd_tab[i].heredoc = 0;
    // (*t_cmd)->cmd_tab[i].errfd = -1;
	t_list *temp = *shell;
	while (*shell != NULL)
	{
		if ((*shell)->state == NORMAL || (*shell)->state == DOUBLEQUOTE || (*shell)->state == SINGLEQUOTE)
			(*t_cmd)->cmd_tab[i].cmd_args = ft_add_double_tab((*shell)->str, (*t_cmd)->cmd_tab[i].cmd_args);
		if ((*shell)->state == INFILE && (*t_cmd)->cmd_tab[i].input_failed == 0)
		{
			if (prev_infd != -1)
				close(prev_infd);
			(*t_cmd)->cmd_tab[i].infd = open((*shell)->str, O_RDONLY, 0644);
			if ((*t_cmd)->cmd_tab[i].infd < 0)
			{
				// perror((*shell)->str);
				if ((*t_cmd)->cmd_tab[i].input_failed == 0 && (*t_cmd)->cmd_tab[i].output_failed == 0)
					perror((*shell)->str);
					// (*t_cmd)->cmd_tab[i].in_str = (*shell)->str;
				(*all)->exit_status = 1; // Set exit status to indicate error
				(*t_cmd)->cmd_tab[i].infd = -1; // Marquer comme échec
				(*t_cmd)->cmd_tab[i].input_failed = 1; // Marquer que la redirection a échoué
				// Ne pas retourner -1, continuer le parsing
			}
			else
				prev_infd = (*t_cmd)->cmd_tab[i].infd;
		}


		if ((*shell)->state == LIMITER && (*t_cmd)->cmd_tab[i].input_failed == 0)
		{
			if (prev_infd != -1)
				close(prev_infd);
			(*t_cmd)->cmd_tab[i].infd = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);
			if ((*t_cmd)->cmd_tab[i].infd < 0)
			{
				// perror((*shell)->str);
				if ((*t_cmd)->cmd_tab[i].input_failed == 0 && (*t_cmd)->cmd_tab[i].output_failed == 0)
					perror((*shell)->str);
					// (*t_cmd)->cmd_tab[i].in_str = (*shell)->str;
				(*all)->exit_status = 1; // Set exit status to indicate error
				(*t_cmd)->cmd_tab[i].infd = -1; // Marquer comme échec
				(*t_cmd)->cmd_tab[i].input_failed = 1; // Marquer que la redirection a échoué
				// Ne pas retourner -1, continuer le parsing
			}
			else
			{
				// prev_infd = (*t_cmd)->cmd_tab[i].infd;
				(*t_cmd)->cmd_tab[i].heredoc++;
				if (access("temp", F_OK) == 0)
				{
					close((*t_cmd)->cmd_tab[i].infd);
					unlink("temp");
				}
				(*t_cmd)->cmd_tab[i].infd  = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);
				char *test = get_next_line(0);
				while (test)
				{
					if (ft_strncmp(test, (*shell)->str, ft_strlen((*shell)->str)) == 0)
					{
						free(test);
						break ;
					}
					// printf("infd:%d\n", (*t_cmd)->cmd_tab[i].infd);
					write((*t_cmd)->cmd_tab[i].infd, test, ft_strlen(test));
					free(test);
					test = get_next_line(0);
				}
				close((*t_cmd)->cmd_tab[i].infd);
				(*t_cmd)->cmd_tab[i].infd = open("temp",  O_RDONLY , 0644);
				prev_infd = (*t_cmd)->cmd_tab[i].infd;
			}	
		}




		if ((*shell)->state == OUTFILE && (*t_cmd)->cmd_tab[i].output_failed == 0 && (*t_cmd)->cmd_tab[i].input_failed == 0)
		{
			if (prev_outfd != -1)
				close(prev_outfd);
			(*t_cmd)->cmd_tab[i].outfd = open((*shell)->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if ((*t_cmd)->cmd_tab[i].outfd < 0)
			{
				// perror((*shell)->str);
				if ((*t_cmd)->cmd_tab[i].output_failed == 0 && (*t_cmd)->cmd_tab[i].input_failed == 0)
					perror((*shell)->str);
					// (*t_cmd)->cmd_tab[i].out_str = (*shell)->str;
				(*all)->exit_status = 1; // Set exit status to indicate error
				(*t_cmd)->cmd_tab[i].outfd = -1; // Marquer comme échec
				(*t_cmd)->cmd_tab[i].output_failed = 1;
				// Ne pas retourner -1, continuer le parsing
			}
			else
				prev_outfd = (*t_cmd)->cmd_tab[i].outfd;
		}
		if ((*shell)->state == OUTFILEAPPEND && (*t_cmd)->cmd_tab[i].output_failed == 0 && (*t_cmd)->cmd_tab[i].input_failed == 0)
	    {
	        if (prev_outfd != -1)
	            close(prev_outfd);
	        (*t_cmd)->cmd_tab[i].outfd = open((*shell)->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
	        if ((*t_cmd)->cmd_tab[i].outfd < 0)
	        {
	            // perror((*shell)->str);
	            if ((*t_cmd)->cmd_tab[i].output_failed == 0 && (*t_cmd)->cmd_tab[i].input_failed == 0)
	            {
	            	perror((*shell)->str);
	            	// (*t_cmd)->cmd_tab[i].out_str = (*shell)->str;
	            }
				(*all)->exit_status = 1; // Set exit status to indicate error
				(*t_cmd)->cmd_tab[i].outfd = -1; // Marquer comme échec
				(*t_cmd)->cmd_tab[i].output_failed = 1;
				// Ne pas retourner -1, continuer le parsing
	        }
			else
	        	prev_outfd = (*t_cmd)->cmd_tab[i].outfd;
	    }
		if ((*shell)->state == PIPE)
		{
			i++;
			// Initialiser les champs pour la nouvelle commande
			(*t_cmd)->cmd_tab[i].infd = -1;
			(*t_cmd)->cmd_tab[i].outfd = -1;
			(*t_cmd)->cmd_tab[i].input_failed = 0;
			(*t_cmd)->cmd_tab[i].output_failed = 0;
			(*t_cmd)->cmd_tab[i].in_str = NULL;
			(*t_cmd)->cmd_tab[i].out_str = NULL;
			// Réinitialiser les variables de suivi des fd précédents
			prev_infd = -1;
			prev_outfd = -1;
		}
		(*shell) = (*shell)->next;
	}
	(*shell) = temp;
	return (0);
}
