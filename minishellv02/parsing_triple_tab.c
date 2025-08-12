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
#include <errno.h>  // Pour errno et EINTR

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
		{
			(*t_cmd)->cmd_tab[i].cmd_args = ft_add_double_tab((*shell)->str, (*t_cmd)->cmd_tab[i].cmd_args);
			if (!(*t_cmd)->cmd_tab[i].cmd_args)
				return (*shell = temp, -2);
		}
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
				signal(SIGINT, SIG_IGN);
				// signal(SIGQUIT, SIG_IGN);
				(*t_cmd)->cmd_tab[i].id_here_doc = fork();
				if ((*t_cmd)->cmd_tab[i].id_here_doc == 0)
				{
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_IGN);
					// prev_infd = (*t_cmd)->cmd_tab[i].infd;

					if (tcgetattr(STDIN_FILENO, &(*all)->term) == 0)
                    {
                        (*all)->term.c_lflag &= ~0001000; // Désactiver ECHOCTL pour ne pas afficher les caractères de contrôle
                        tcsetattr(STDIN_FILENO, TCSANOW,  &(*all)->term);
                    }

					(*t_cmd)->cmd_tab[i].heredoc++;
					if (access("temp", F_OK) == 0)
					{
						close((*t_cmd)->cmd_tab[i].infd);
						unlink("temp");
					}
					(*t_cmd)->cmd_tab[i].infd  = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);
					//protec open

					char *test = get_next_line(0);
					while (test && g_sigvaleur == 0)
					{
						if (ft_strncmp(test, (*shell)->str, ft_strlen((*shell)->str)) == 0)
						{
							free(test);
							break ;
						}
						write((*t_cmd)->cmd_tab[i].infd, test, ft_strlen(test));
						free(test);
						test = get_next_line(0);
					}

					close((*t_cmd)->cmd_tab[i].infd);
					exit(0);  // IMPORTANT: Sortir du processus enfant
				}
				else
				{
					// Processus parent: attendre la fin du heredoc
					int status;
					waitpid((*t_cmd)->cmd_tab[i].id_here_doc, &status, 0);
					// printf("signal0");
					signal(SIGINT, ft_test);
					if (WIFSIGNALED(status))
					{
						// printf("signal1");
						int sig = WTERMSIG(status);
						if (sig == SIGINT)
						{
							// printf("signal2");
							write (1, "^C\n", 3);
							(*all)->exit_status = 128 + sig;
							return (-1);
						}
					}
					// Ouvrir le fichier temp en lecture pour la commande
					close((*t_cmd)->cmd_tab[i].infd);  // Fermer le fd d'écriture
					// unlink("temp");
					(*t_cmd)->cmd_tab[i].infd = open("temp", O_RDONLY, 0644);
					if ((*t_cmd)->cmd_tab[i].infd < 0)
					{
						perror("temp");
						(*t_cmd)->cmd_tab[i].input_failed = 1;
					}
					else
					{
						prev_infd = (*t_cmd)->cmd_tab[i].infd;
					}
				}
				// signal(SIGINT, ft_test);
				// if (g_sigvaleur == 1)
				// {

				// }
			
			}	
				// prev_infd = (*t_cmd)->cmd_tab[i].infd;
				// (*all)->t_cmd->cmd_tab[]

				/*
				(*t_cmd)->cmd_tab[i].id_here_doc = fork();
				if ((*t_cmd)->cmd_tab[i].id_here_doc == 0)
				{

					signal(SIGINT, SIG_DFL);                	
                	signal(SIGQUIT, SIG_DFL);
					
					(*t_cmd)->cmd_tab[i].heredoc++;
					if (access("temp", F_OK) == 0)
					{
						close((*t_cmd)->cmd_tab[i].infd);
						unlink("temp");
					}
					int heredoc_test = 0;
					(*t_cmd)->cmd_tab[i].infd  = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);

					// Reset errno avant get_next_line pour détecter les interruptions
					errno = 0;
					char *test = get_next_line(0);
					// char *test = readline("test>");
					if (g_sigvaleur == 1)
					{
						printf("1DEBUG:g_sig:%d\n", g_sigvaleur);
						if (test)
							free(test);
						unlink("temp");
						return (-1);
					}
					if (!test) // Ctrl+D (EOF) - mais peut aussi être une interruption de signal
					{
						// Vérifier si c'est un vrai EOF ou une interruption de signal
						if (g_sigvaleur == 1 || errno == EINTR)
						{
							printf("Signal interrompu pendant get_next_line (errno=%d)\n", errno);
							unlink("temp");
							return (-1);
						}
						printf("warning: here-document delimited by end-of-file (wanted `%s')\n", (*shell)->str);
						heredoc_test = 1;
					}

					while (test && g_sigvaleur == 0 && heredoc_test == 0)
					{
						if (ft_strncmp(test, (*shell)->str, ft_strlen((*shell)->str)) == 0)
						{
							free(test);
							break ;
						}
						// printf("infd:%d\n", (*t_cmd)->cmd_tab[i].infd);
						write((*t_cmd)->cmd_tab[i].infd, test, ft_strlen(test));
						free(test);
						
						// Vérifier le signal AVANT d'appeler get_next_line
						if (g_sigvaleur == 1)
						{
							printf("2DEBUG:g_sig:%d (avant get_next_line)\n", g_sigvaleur);
							unlink("temp");
							return (-1);
						}
						
						// Reset errno avant chaque get_next_line
						errno = 0;
						test = get_next_line(0);
						
						// Vérifier le signal APRÈS get_next_line
						if (g_sigvaleur == 1)
						{
							printf("2DEBUG:g_sig:%d (après get_next_line)\n", g_sigvaleur);
							if (test)
								free(test);
							unlink("temp");
							return (-1);
						}
						if (!test) // Ctrl+D (EOF) ou interruption signal
						{
							if (g_sigvaleur == 1 || errno == EINTR)
							{
								printf("Signal interrompu pendant get_next_line (boucle, errno=%d)\n", errno);
								unlink("temp");
								return (-1);
							}
							printf("warning: here-document delimited by end-of-file (wanted `%s')\n", (*shell)->str);
							heredoc_test = 1;
						}
					}
					printf("3DEBUG:g_sig:%d\n", g_sigvaleur);
					close((*t_cmd)->cmd_tab[i].infd);
					if (g_sigvaleur == 1)
					{
						printf("4DEBUG:g_sig:%d\n", g_sigvaleur);
						unlink("temp");
						return (-1);
					}
					(*t_cmd)->cmd_tab[i].infd = open("temp",  O_RDONLY , 0644);
					prev_infd = (*t_cmd)->cmd_tab[i].infd;
					unlink("temp");
					printf("5DEBUG:g_sig:%d\n", g_sigvaleur);
					*/
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
/*
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
		{
			(*t_cmd)->cmd_tab[i].cmd_args = ft_add_double_tab((*shell)->str, (*t_cmd)->cmd_tab[i].cmd_args);
			if (!(*t_cmd)->cmd_tab[i].cmd_args)
				return (-2);
		}
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
			// else
			// {
			// 	// prev_infd = (*t_cmd)->cmd_tab[i].infd;
			// 	(*t_cmd)->cmd_tab[i].heredoc++;
			// 	if (access("temp", F_OK) == 0)
			// 	{
			// 		close((*t_cmd)->cmd_tab[i].infd);
			// 		unlink("temp");
			// 	}
			// 	(*t_cmd)->cmd_tab[i].infd  = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);
			// 	char *test = get_next_line(0);
			// 	while (test)
			// 	{
			// 		if (ft_strncmp(test, (*shell)->str, ft_strlen((*shell)->str)) == 0)
			// 		{
			// 			free(test);
			// 			break ;
			// 		}
			// 		// printf("infd:%d\n", (*t_cmd)->cmd_tab[i].infd);
			// 		write((*t_cmd)->cmd_tab[i].infd, test, ft_strlen(test));
			// 		free(test);
			// 		test = get_next_line(0);
			// 	}
			// 	close((*t_cmd)->cmd_tab[i].infd);
			// 	(*t_cmd)->cmd_tab[i].infd = open("temp",  O_RDONLY , 0644);
			// 	prev_infd = (*t_cmd)->cmd_tab[i].infd;
			// }	
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

				// signal(SIGINT, SIG_DFL); 

				char *test = get_next_line(0);
				while (test && g_sigvaleur == 0)
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

				// signal(SIGINT, ft_test);


				close((*t_cmd)->cmd_tab[i].infd);
				(*t_cmd)->cmd_tab[i].infd = open("temp",  O_RDONLY , 0644);
				prev_infd = (*t_cmd)->cmd_tab[i].infd;
			
					
				// (*t_cmd)->cmd_tab[i].infd  = open("temp", O_WRONLY | O_CREAT | O_APPEND, 0644);
				
				// // Configuration des signaux pour heredoc
				// // On garde notre handler personnalisé pour maintenir g_sigvaleur
				// // signal(SIGINT, SIG_DFL);  // Commenté - on garde ft_test
				
				// char *test = readline("> ");
				// while (test && g_sigvaleur == 0)  // Vérifier g_sigvaleur dans la boucle
				// {
				// 	if (ft_strncmp(test, (*shell)->str, ft_strlen((*shell)->str)) == 0 
				// 		&& ft_strlen(test) == ft_strlen((*shell)->str))
				// 	{
				// 		free(test);
				// 		break ;
				// 	}
				// 	// printf("infd:%d\n", (*t_cmd)->cmd_tab[i].infd);
				// 	write((*t_cmd)->cmd_tab[i].infd, test, ft_strlen(test));
				// 	write((*t_cmd)->cmd_tab[i].infd, "\n", 1);  // Ajouter le newline
				// 	free(test);
				// 	test = readline("> ");
				// }
				
				// // Restaurer les signaux
				// // signal(SIGINT, ft_test);  // Pas besoin de restaurer car on n'a pas changé
				
				// Vérifier si un signal a été reçu pendant la saisie
				if (g_sigvaleur == 1)
				{
					close((*t_cmd)->cmd_tab[i].infd);
					unlink("temp");
					printf("\n");  // Afficher une nouvelle ligne après l'interruption
					g_sigvaleur = 0;
					// Mettre à jour exit_status pour SIGINT
					(*all)->exit_status = 130;  // Code pour SIGINT dans bash
					
					return (-1);  // Retourner -1 pour continuer au prompt suivant
				}
				
				if (!test)  // EOF ou signal pendant readline (Ctrl+C ou Ctrl+D)
				{
					close((*t_cmd)->cmd_tab[i].infd);
					unlink("temp");
					printf("\n");  // Afficher une nouvelle ligne après l'interruption
					g_sigvaleur = 0;
					// Mettre à jour exit_status pour SIGINT
					(*all)->exit_status = 130;  // Code pour SIGINT dans bash
					
					// Restaurer les signaux avant de retourner
					// signal(SIGINT, ft_test);
					
					return (-1);  // Retourner -1 pour continuer au prompt suivant
				}
				
				close((*t_cmd)->cmd_tab[i].infd);
				// (*t_cmd)->cmd_tab[i].infd = open("temp",  O_RDONLY , 0644);
				// prev_infd = (*t_cmd)->cmd_tab[i].infd;
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
*/