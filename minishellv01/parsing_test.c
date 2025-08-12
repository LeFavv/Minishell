/* ************************************************************************** */
/*                                                                            */
/*		                                                  :::      ::::::::   */
/*   parsing_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07														  */
/*   Updated: 2025/07/18 11:38:32 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sigvaleur;
// if (i == 0)
// {
// 	if (t_cmd->cmd_tab[i].infd >= 0)
// 		dup2(t_cmd->cmd_tab[i].infd, 0);
// 	if (t_cmd->nbr_cmd > 1)
// 		dup2(t_cmd->cmd_tab[i].fd[1], 1);
// 	else if (t_cmd->cmd_tab[i].outfd >= 0)
// 		dup2(t_cmd->cmd_tab[i].outfd, 1);
// }
// else if (i == t_cmd->nbr_cmd - 1)
// {
// 	dup2(t_cmd->cmd_tab[i - 1].fd[0], 0);
// 	if (t_cmd->cmd_tab[i].outfd >= 0)
// 		dup2(t_cmd->cmd_tab[i].outfd, 1);
// }
// else
// {
// 	dup2(t_cmd->cmd_tab[i - 1].fd[0], 0);
// 	dup2(t_cmd->cmd_tab[i].fd[1], 1);
// }

int ft_close_fd(t_all **all)
{
	int i;

	i = 0;
	while (i < (*all)->t_cmd->nbr_cmd)
	{
		if ((*all)->t_cmd->cmd_tab[i].infd != -1)
			close((*all)->t_cmd->cmd_tab[i].infd);
		if ((*all)->t_cmd->cmd_tab[i].outfd != -1)
			close((*all)->t_cmd->cmd_tab[i].outfd);
		i++;
	}
	return (0);
}

//execute les commandes 
int ft_exec_commande(t_commande *t_cmd, t_redir *t_red, t_all **all, char **env)
{
	int i;
	(void)t_red;
	(void)all;
	i = 0;
	while (i < t_cmd->nbr_cmd)
	{
		// Vérifier si la commande existe et a des arguments
		if (!t_cmd->cmd_tab[i].cmd_args || !t_cmd->cmd_tab[i].cmd_args[0])
		{
			t_cmd->cmd_tab[i].id1 = -1;
			i++;
			continue;
		}
		
		// Vérifier si la commande a des redirections d'entrée qui ont échoué
		if (t_cmd->cmd_tab[i].input_failed == 1 || t_cmd->cmd_tab[i].output_failed == 1)
		{
			// if (t_cmd->cmd_tab[i].input_failed == 1)
			// {
			// 	perror(t_cmd->cmd_tab[i].in_str);
			// }
			// if (t_cmd->cmd_tab[i].output_failed == 1)
			// {
			// 	perror(t_cmd->cmd_tab[i].out_str);
			// 	// ft_err(t_cmd->cmd_tab[i].out_str, "Permission denied");
			// }
			// Ne pas exécuter la commande si redirection d'entrée a échoué
			t_cmd->cmd_tab[i].id1 = -1; // Marquer comme non créé
			i++;
			continue ;
		}

		//condition pour verifier uniquement si les builtin existent
		//puis dup2
		//puis executer les fonctions
		if (ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "echo", 4) == 0
			|| ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "pwd", 3) == 0
			|| ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "env", 3) == 0
			|| (ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "export", 6) == 0 &&  t_cmd->nbr_cmd > 1)
			|| (ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "exit", 5) == 0 && t_cmd->nbr_cmd > 1))
		{

			// printf("builtin\n");
			t_cmd->cmd_tab[i].id1 = fork();
			if (t_cmd->cmd_tab[i].id1 == 0)
			{
				// Restaurer les signaux par défaut pour les processus enfants
				signal(SIGINT, SIG_DFL);   // Comportement par défaut pour SIGINT
				signal(SIGQUIT, SIG_DFL);  // Comportement par défaut pour SIGQUIT
				
				if (t_cmd->cmd_tab[i].infd >= 0)
					dup2(t_cmd->cmd_tab[i].infd, 0);
				else if (i > 0)  // Si pas de redirection d'entrée, utiliser le pipe précédent
					dup2(t_cmd->cmd_tab[i - 1].fd[0], 0);
				
				// Gestion des redirections de sortie pour toutes les commandes
				if (t_cmd->cmd_tab[i].outfd >= 0)
				{
					// printf("redirection sortie pour commande %d\n", i);
					dup2(t_cmd->cmd_tab[i].outfd, 1);
				}
				else if (i < t_cmd->nbr_cmd - 1)  // Si pas de redirection de sortie, utiliser le pipe suivant
					dup2(t_cmd->cmd_tab[i].fd[1], 1);
				// Sinon, la sortie reste stdout (cas de la dernière commande sans redirection)
				
				ft_close_pipe(t_cmd);
				ft_close_fd(all);
				if (is_builtin_3(t_cmd->cmd_tab[i].cmd_args, all) == 1)
				{
					// (*all)->exit_status = 0;
				}
				// else
				// {
				// 	(*all)->exit_status = 1;
				// }
				// ft_close_pipe(t_cmd);
				t_cmd->cmd_tab[i].id1 = -1; // Les builtins n'ont pas de processus fils
				if (ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "exit", 4) == 0)
                   	exit(ft_atoi(t_cmd->cmd_tab[i].cmd_args[1]) % 256);
				exit((*all)->exit_status);
			}
		}
		else if (
			ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "export", 6) == 0
			|| ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "unset", 5) == 0
			|| ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "cd", 2) == 0
			// || ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "pwd", 3) == 0
			// || ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "exit", 4) == 0
			// || ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "env", 3) == 0
			)
		{
            if (is_builtin_3(t_cmd->cmd_tab[i].cmd_args, all) == 1)
            {
                // printf("je suis la\n");
                // printf("exit1:%d\n", (*all)->exit_status);
                // (*all)->exit_status = homemade_cd(t_cmd->cmd_tab[i].cmd_args, all);
                // printf("exit2:%d\n", (*all)->exit_status);
                t_cmd->cmd_tab[i].id1 = fork();
                if (t_cmd->cmd_tab[i].id1 == 0)
                {
                    // Restaurer les signaux par défaut pour les processus enfants
                    signal(SIGINT, SIG_DFL);   // Comportement par défaut pour SIGINT
                    signal(SIGQUIT, SIG_DFL);  // Comportement par défaut pour SIGQUIT
                    
                    ft_close_pipe(t_cmd);
                    ft_close_fd(all);
                    if (ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "exit", 4) == 0)
                    	exit(ft_atoi(t_cmd->cmd_tab[i].cmd_args[1]) % 256);
                    exit((*all)->exit_status);
                }
            }
		}
		else if (ft_strncmp(t_cmd->cmd_tab[i].cmd_args[0], "exit", 4) == 0 && t_cmd->nbr_cmd == 1)
		{
			// ft_exit()
			if (!t_cmd->cmd_tab[i].cmd_args[1])
			{
				write(1, "exit\n", 5); //sortie ou 2 
				exit((*all)->exit_status);
			}
			if (!ft_is_digit(t_cmd->cmd_tab[i].cmd_args[1])
				|| ft_atoi(t_cmd->cmd_tab[i].cmd_args[1]) > INT_MAX
				|| ft_atoi(t_cmd->cmd_tab[i].cmd_args[1]) < INT_MIN)
            {
                write(1, "exit\n", 5); //sortie 1 ou 2 
                ft_err(t_cmd->cmd_tab[i].cmd_args[1], "numeric argument required");
                exit(2);
            }
			if (t_cmd->cmd_tab[i].cmd_args[2])
			{
				write(1, "exit\n", 5); //sortie 1 ou 2 
				ft_err(t_cmd->cmd_tab[i].cmd_args[1], "too many arguments");
				(*all)->exit_status = 1;
				t_cmd->cmd_tab[i].id1 = fork();
				if (t_cmd->cmd_tab[i].id1 == 0)
				{
					// Restaurer les signaux par défaut pour les processus enfants
					// setup_child_signals();
					
					ft_close_pipe(t_cmd);
					ft_close_fd(all);
					exit(1);
				}
				// return (1);
			}
			if ((*all)->t_cmd->nbr_cmd == 1 && t_cmd->cmd_tab[i].cmd_args[2] == NULL)
				exit(ft_atoi(t_cmd->cmd_tab[i].cmd_args[1]) % 256);
			// return (0);
		}
		else
		{	
			t_cmd->cmd_tab[i].id1 = fork();
			if (t_cmd->cmd_tab[i].id1 == 0)
			{
				 // Restaurer les signaux par défaut pour les processus enfants
               // if  (signal(SIGINT, SIG_DFL))
               // {
               // 	g_sigvaleur = 1;
               // } 
               // 	// Comportement par défaut pour SIGINT
               	signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);  // Comportement par défaut pour SIGQUIT
				
				// printf("J'arrive la \n");
				// Gestion des redirections d'entrée pour toutes les commandes
				// printf("infd:%d\n", (*t_cmd).cmd_tab[i].infd);
				if (t_cmd->cmd_tab[i].infd >= 0)
					dup2(t_cmd->cmd_tab[i].infd, 0);
				else if (i > 0)  // Si pas de redirection d'entrée, utiliser le pipe précédent
					dup2(t_cmd->cmd_tab[i - 1].fd[0], 0);
				
				// Gestion des redirections de sortie pour toutes les commandes
				if (t_cmd->cmd_tab[i].outfd >= 0)
				{
					// printf("redirection sortie pour commande %d\n", i);
					dup2(t_cmd->cmd_tab[i].outfd, 1);
				}
				else if (i < t_cmd->nbr_cmd - 1)  // Si pas de redirection de sortie, utiliser le pipe suivant
					dup2(t_cmd->cmd_tab[i].fd[1], 1);
				// Sinon, la sortie reste stdout (cas de la dernière commande sans redirection)
				
				ft_close_pipe(t_cmd);
				ft_close_fd(all);
				if (exec(t_cmd->cmd_tab[i].cmd_args, env) == -1)
					exit(127);
				// ft_close_fd(all);
				exit(0);
			}
			// printf("J'arrive la \n");
			// close((*t_cmd).cmd_tab[i].infd);
			// unlink("temp");
		}
		// printf("J'arrive la \n");
		// close((*t_cmd).cmd_tab[i].infd);
		// unlink("temp");
		i++;
	}
	ft_close_pipe(t_cmd);
	ft_close_fd(all);
	return (0);
}

//pas encore utiliser
//attribue les state CMD ou ARG sur les token
void ft_assign_cmd_arg_states(t_list **lst)
{
	t_list *temp;
	int first_cmd;

	if (!lst || !*lst)
		return;
	temp = *lst;
	first_cmd = 1;
	while (*lst)
	{
		if ((*lst)->state == PIPE)
			first_cmd = 1;
		else if ((*lst)->state == NORMAL || (*lst)->state == DOUBLEQUOTE || (*lst)->state == SINGLEQUOTE)
		{
			if (first_cmd)
			{
				(*lst)->state = CMD;
				first_cmd = 0;
			}
			else
				(*lst)->state = ARG;
		}
		*lst = (*lst)->next;
	}
	*lst = temp;
}

char **create_default_env(void)
{
    char **tab = NULL;
    char    *str = NULL;

    char    cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd)))
        return (NULL);
    str = ft_strjoin("PWD=", cwd); // non free
	if (!str)
		return (NULL);
    tab = ft_add_double_tab(str, tab);
	if (!tab)
		return (free(str), NULL);
    tab = ft_add_double_tab("_=/usr/bin/env", tab);
	if (!tab)
		return (free(str), NULL);
	tab = ft_add_double_tab("SHLVL=1", tab);
	if (!tab)
		return (free(str), NULL);
	// tab = ft_add_double_tab("PATH=/home/kevwang/.local/funcheck/host:/home/kevwang/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin", tab);
	tab = ft_add_double_tab("PATH=/bin", tab);
	if (!tab)
		return (free(str), NULL);
	free(str); // Libérer str avant de retourner
	return (tab);
}

void ft_concatenate(t_list **lst)
{
	// t_list *current;
	// t_list *next;
	t_list *temp;
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
			(*lst)->next = (*lst)->next->next;
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

//Seulement mettre à jour l'exit status si un processus a réellement été exécuté
//Si aucun processus n'a été exécuté mais qu'aucune erreur n'a été détectée, exit_status = 0
//Si aucun processus n'a été exécuté à cause d'erreurs, garder l'exit_status précédent
void ft_check_exit_status(t_all **all)
{
	int process_executed = 0;
	int j = 0;
	while (j < (*all)->t_cmd->nbr_cmd)
	{
		if ((*all)->t_cmd->cmd_tab[j].id1 > 0)
		{
			process_executed = 1;
			break;
		}
		j++;
	}
	if (process_executed && WIFEXITED((*all)->t_cmd->status))
		(*all)->exit_status = WEXITSTATUS((*all)->t_cmd->status);
	else if (process_executed && WIFSIGNALED((*all)->t_cmd->status))
	{
		int sig = WTERMSIG((*all)->t_cmd->status);
		if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
		else if (sig == SIGINT)
			write (1, "\n", 1);
		if (sig != SIGPIPE)
			(*all)->exit_status = 128 + sig; //128 + le code du signal
	}
	else if (process_executed == 0)
	{
		int has_error = 0;
		j = 0;
		while (j < (*all)->t_cmd->nbr_cmd)
		{
			if ((*all)->t_cmd->cmd_tab[j].input_failed == 1
				|| (*all)->t_cmd->cmd_tab[j].output_failed == 1)
			{
				has_error = 1;
				break;
			}
			j++;
		}
		if (has_error == 0)
			(*all)->exit_status = 0;
	}
}

// Vérifier si au moins une commande a des arguments
// Pas de commande valide, nettoyer et return(-1) aui va continue ;
int ft_check_arg(t_all **all)
{
	int has_valid_cmd = 0;
	int j = 0;

	while (j < (*all)->t_cmd->nbr_cmd)
	{
		if ((*all)->t_cmd->cmd_tab[j].cmd_args && (*all)->t_cmd->cmd_tab[j].cmd_args[0])
		{
			has_valid_cmd = 1;
			break;
		}
		j++;
	}
	if (!has_valid_cmd)
	{
		j = 0;
		while (j < (*all)->t_cmd->nbr_cmd && (*all)->t_cmd->cmd_tab[j].cmd_args)
		{
			ft_free_double_tab((*all)->t_cmd->cmd_tab[j].cmd_args);
			j++;
		}
		free((*all)->t_cmd->cmd_tab);
		free((*all)->t_cmd);
		free((*all)->str);
		if ((*all)->shell)
		{
			ft_clear(&(*all)->shell);
		}
		return (-1);
	}
	return (0);
}

int ft_init_triple_tab(t_all **all)
{
	(*all)->t_cmd = malloc(sizeof(t_commande));
	if (!(*all)->t_cmd)
		return (-2);
	(*all)->t_cmd->nbr_cmd = ft_count_commands((*all)->shell);
	(*all)->t_cmd->cmd_tab = malloc(sizeof(t_cmd_tab) * (*all)->t_cmd->nbr_cmd);
	if (!(*all)->t_cmd->cmd_tab)
		return (-2);
	ft_set_triple_tab_null((*all)->t_cmd);
	return (0);
}

int ft_check_parse(t_all **all)
{
	// (*all)->str = replace_dollar_test2((*all)->str, (*all)->env, *all);
	char *temp;
	
	temp = replace_dollar_pour_de_vrai((*all)->str, *all);
	free((*all)->str);
	(*all)->str = temp;
	int parse_result = ft_parse_decoupe((*all)->str, &(*all)->shell, (*all));
	if (parse_result == -1)
	{
		// Skip this iteration if parsing failed due to unclosed quotes
		free((*all)->str);
		if ((*all)->shell)
			ft_clear(&(*all)->shell);
		// ft_free_(*all)((*all));
		// continue;
		return (-1);
	}
	else if (parse_result == -2)
	{
		// M(*all)oc failure - exit program
		free((*all)->str);
		if ((*all)->shell)
			ft_clear(&(*all)->shell);
		ft_err("minishell", "malloc failed");
		// exit(1);
		return (-2);
	}
	return (0);
}
/*
int ft_read_input(t_all **all)
{
	(*all)->str = readline("CacaTest > ");
	
	if (!(*all)->str) // Ctrl+D (EOF)
	{
		printf("exit\n");
		return (-1);
	}
	if (!(*all)->str[0]) // Chaîne vide
	{
		if ((*all)->str)
			free((*all)->str);
		// printf("test:%s\n", (*all)->str);
		return (-2);
	}
	
	// Si on arrive ici, l'utilisateur a tapé quelque chose de valide
	// Reset le flag de signal car on a une commande valide
	g_sigvaleur = 0;
	
	add_history((*all)->str);
	return (0);
}*/

int ft_read_input(t_all **all)
{
	(*all)->str = readline("CacaTest > ");
	
	if (!(*all)->str) // Ctrl+D (EOF)
	{
		printf("exit\n");
		return (-1);
	}
	if (!(*all)->str[0]) // Chaîne vide
	{
		free((*all)->str);
		return (-2);
	}

	// Si on arrive ici, une commande valide a été saisie
	// Reset g_sigvaleur car nous avons une vraie commande à traiter
	g_sigvaleur = 0;
	
	add_history((*all)->str);
	return (0);
}

int ft_parse(t_all **all)
{
//Parse_decoupe bah elle decoupe l'input en liste chaine
	int parse_result = ft_check_parse(all);
	if (parse_result == -1)
		return (-1); // Continue si la parsing a échoué à cause de guillemets non fermés
	else if (parse_result == -2)
		return (-2); // Malloc failure - exit program
	ft_concatenate(&(*all)->shell);
// ft_print(all->shell);
	if (ft_lstiter_env(&(*all)->shell, (*all)->env, *all) == -1)
	{
		ft_free_all(*all);
		return (-1);
	}
// ft_print((*all)->shell);
	if (ft_init_triple_tab(all) == -2)
		return (ft_free_all(*all), -2);
	parse_result = ft_create_triple_tab(&(*all)->shell, &(*all)->t_cmd, all);
	if (parse_result == -2)
		return (ft_free_all(*all), -2);
	if (parse_result == -1)
		return (ft_free_all(*all), -1);
	if (ft_check_arg(all) == -1)
		return (-1);
	return (0);
}

int ft_all(t_all **all)
{
	(*all)->shell = NULL;
	// int read_result = ft_read_input(all);
	// if (read_result == -1)
	// 	return (-1); // Exit if EOF (Ctrl+D) is detected
	// else if (read_result == -2)
	// 	return (-2); // Continue if empty string is detected

	// int parse_result = ft_parse(all);
	// if (parse_result == -1)
	// 	return (-2); // Continue if parsing failed due to unclosed quotes
	// else if (parse_result == -2)
	// 	return (-1); // Malloc failure - exit program

// ft_print_triple_tab(all->t_cmd);
	// if (ft_open_pipe((*all)->t_cmd) == 1)
	// 	return (-1);
	// ft_exec_commande((*all)->t_cmd, (*all)->t_red, all, (*all)->env);
	// ft_waitpid((*all)->t_cmd);
	// ft_close_pipe((*all)->t_cmd);
	// ft_check_exit_status(all);
	// ft_free_all(*all);
	return (0);
}

void set_exit(int *exit2)
{
	static int *new_exit = NULL;

	if (new_exit == NULL)
		new_exit = exit2;
	
	*new_exit = 130;
}

void ft_test(int signum)
{
	(void)signum;
	g_sigvaleur = 1; // IMPORTANT : Marquer qu'un signal sigint a été reçu
	write(1, "\n", 1);
    rl_replace_line("", 0);      // Vider la ligne courante
    rl_on_new_line();            // Indiquer qu'on est sur une nouvelle ligne
    rl_redisplay();              // Réafficher le prompt
    set_exit(&g_sigvaleur);
}

void ft_sigquit(int signum)
{
	(void)signum;
	// Pour SIGQUIT (Ctrl+\) au prompt : ne rien faire (comme bash)
	// Le signal sera géré par les processus enfants avec SIG_DFL
}

// int ft_all()

//caca parsing_test.c pipex_path.c parsing_dollar.c minishell_utils.c ft_strjoin.c ft_split.c ft_itoa.c -lreadline -o minishell
int main(int argc, char **argv, char **env)
{
	(void)argv;
	(void)env;
	t_all *all;

	if (argc == 1)
	{
		all = malloc(sizeof(t_all));
		if (!all)
			return (1);
		
		// Initialiser tous les pointeurs à NULL pour ft_free_all
		all->str = NULL;
		all->shell = NULL;
		all->t_cmd = NULL;
		all->env = NULL;
		all->exit_status_char = NULL;
		all->exit_status = 0;
		
		// Configuration de l'environnement
		if (env[0])
		{
        	all->env = ft_copy_double_tab(env);
        	if (!all->env)
			{
				ft_free_all(all);
				free(all);
				return (1);
			}
			ft_shlvl(&all);
		}
		else
		{
			all->env = create_default_env();
			if (!all->env)
			{
				ft_err("minishell", "malloc failed");
				ft_free_all(all);
				free(all);
				return (1);
			}
		}
		g_sigvaleur = 0;
		set_exit(&(all)->exit_status);

		// Configuration des signaux avec signal() au lieu de sigaction()
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, ft_test);
		
		all->exit_status = 0; // Initialiser l'exit status à 0 au début du programme
		while (1)
		{
			all->shell = NULL;

			if (tcgetattr(STDIN_FILENO, &all->term) == 0)
            {
                all->term.c_lflag |= ECHOCTL;
                tcsetattr(STDIN_FILENO, TCSANOW, &all->term);
            }

	
			int read_result = ft_read_input(&all);
			if (read_result == -1)
			{
				exit(all->exit_status);
			}
			else if (read_result == -2)
			{
				// Si c'est à cause d'un signal, mettre à jour l'exit status
				if (g_sigvaleur == 1)
				{
					all->exit_status = 130;  // Code pour SIGINT
					g_sigvaleur = 0;         // Reset après traitement
				}
				continue; // Continue if empty string is detected or signal received
				// return (0); // Continue if empty string is detected
			}

			// if (tcgetattr(STDIN_FILENO, &all->term) == 0)
            // {
            //     all->term.c_lflag |= ECHOCTL;
            //     tcsetattr(STDIN_FILENO, TCSANOW, &all->term);
            // }



			int parse_result = ft_parse(&all);
			if (parse_result == -1)
				continue; // Continue if parsing failed due to unclosed quotes
				// return (1); // Continue if parsing failed due to unclosed quotes
			else if (parse_result == -2)
				return (1); // Malloc failure - exit program



			if (ft_open_pipe(all->t_cmd) == 1)
				return (1);
			
			// Ignorer SIGINT pendant l'exécution des commandes enfants
			signal(SIGINT, SIG_IGN);
			
			ft_exec_commande(all->t_cmd, all->t_red, &all, all->env);
			ft_waitpid(all->t_cmd);
			
			// Restaurer le handler SIGINT après l'exécution
			signal(SIGINT, ft_test);
			
			ft_close_pipe(all->t_cmd);
			ft_check_exit_status(&all);
	
			ft_free_all(all);
		}
		free(all);
	}
	return (0);
}

/*
//caca parsing_test.c pipex_path.c parsing_dollar.c minishell_utils.c ft_strjoin.c ft_split.c ft_itoa.c -lreadline -o minishell
int main(int argc, char **argv, char **env)
{
	(void)argv;
	t_all *all;

	if (argc == 1)
	{
		all = malloc(sizeof(t_all));
		if (!all)
			return (1);
		// all->sigint.sa_sigaction = ft_receive;
		// sigaction(SIGINT, &all->sigint, NULL);
		// sigaction(SIGUSR2, &sig, NULL);
		// all->env = env;
		if (env[0])
		{
        	all->env = env;
			ft_shlvl(&all);
		}
		else
		{
			// printf("no env\n");
			all->env = create_default_env();
			if (!all->env)
			{
				ft_err("minishell", "malloc failed");
				free(all);
				return (1);
			}
		}
		// all->pid_str = NULL;
		// all->pid_str = ft_get_pid(all); //peut etre a enlever
		all->exit_status = 0; // Initialiser l'exit status à 0 au début du programme
		while (1)
		{
			all->shell = NULL;

			// if (ft_all(all) == -1)
				// continue ;
			
			// Ne pas réinitialiser exit_status ici, le garder de la commande précédente
			all->str = readline("CacaTest > ");
			if (!all->str) // Ctrl+D (EOF)
			{
				printf("exit\n");
				break;
			}
			if (!all->str[0]) // Chaîne vide
			{
				free(all->str);
				continue;
			}
			add_history(all->str);

			//Parse_decoupe bah elle decoupe l'input en liste chaine
			int parse_result = ft_check_parse(&all);
			if (parse_result == -1)
				continue; // Continue si la parsing a échoué à cause de guillemets non fermés
			else if (parse_result == -2)
				return (1); // Malloc failure - exit program
			// int parse_result = ft_parse_decoupe(all->str, &all->shell, all);
			// if (parse_result == -1)
			// {
			// 	// Skip this iteration if parsing failed due to unclosed quotes
			// 	free(all->str);
			// 	if (all->shell)
			// 		ft_clear(&all->shell);
			// 	// ft_free_all(all);
			// 	continue;
			// }
			// else if (parse_result == -2)
			// {
			// 	// Malloc failure - exit program
			// 	free(all->str);
			// 	if (all->shell)
			// 		ft_clear(&all->shell);
			// 	ft_err("minishell", "malloc failed");
			// 	exit(1);
			// }
			
			ft_concatenate(&all->shell);

			//lstiter_env pour verifier les redirecions '<' '>' '>>' '<<'
	// ft_print(all->shell);
			// ft_lstiter_env(&all->shell, all->env, all);
			if (ft_lstiter_env(&all->shell, all->env, all) == -1)
			{
				if (all->shell)
					ft_clear(&all->shell);
				free(all->str);
				// ft_free_all(all);
				continue ;
			}
	// ft_print(all->shell);

			// ft_assign_cmd_arg_states(&all->shell);

			if (ft_init_triple_tab(&all) == -2)
			{
				// continue;
				// free(all->str);
				// if (all->shell)
				// 	ft_clear(&all->shell);
				// if (all->t_cmd && all->t_cmd->cmd_tab)
				// 	free(all->t_cmd->cmd_tab);
				// if (all->t_cmd)
				// 	free(all->t_cmd);
				return (ft_free_all(all), 1);
			}
			//Compte le nombre de commande
			// all->t_cmd = malloc(sizeof(t_commande));
			// if (!all->t_cmd)
			// 	return (1);
			// all->t_cmd->nbr_cmd = ft_count_commands(all->shell);
			// all->t_cmd->cmd_tab = malloc(sizeof(t_cmd_tab) * all->t_cmd->nbr_cmd);
			// if (!all->t_cmd->cmd_tab)
			// 	return (1);

			//Creation des doubles tableaux pour les commandes
			// ft_set_triple_tab_null(all->t_cmd);
			if (ft_create_triple_tab(&all->shell, &all->t_cmd, &all) == -2)
			{
				// if (all->shell)
				// 	ft_clear(&all->shell);
				// free(all->str);
				// if (all->t_cmd && all->t_cmd->cmd_tab)
				// {
				// 	int j = 0;
				// 	while (j < all->t_cmd->nbr_cmd && all->t_cmd->cmd_tab[j].cmd_args)
				// 	{
				// 		ft_free_double_tab(all->t_cmd->cmd_tab[j].cmd_args);
				// 		j++;
				// 	}
				// 	free(all->t_cmd->cmd_tab);
				// }
				// if (all->t_cmd)
				// 	free(all->t_cmd);
				return (ft_free_all(all), 1);
			}
			
			if (ft_check_arg(&all) == -1)
			{
				continue ;
			}
			// Vérifier si au moins une commande a des arguments
			// int has_valid_cmd = 0;
			// int j = 0;
			// while (j < all->t_cmd->nbr_cmd)
			// {
			// 	if (all->t_cmd->cmd_tab[j].cmd_args && all->t_cmd->cmd_tab[j].cmd_args[0])
			// 	{
			// 		has_valid_cmd = 1;
			// 		break;
			// 	}
			// 	j++;
			// }
			
			// if (!has_valid_cmd)
			// {
			// 	// Pas de commande valide, nettoyer et continuer
			// 	j = 0;
			// 	while (j < all->t_cmd->nbr_cmd && all->t_cmd->cmd_tab[j].cmd_args)
			// 	{
			// 		ft_free_double_tab(all->t_cmd->cmd_tab[j].cmd_args);
			// 		j++;
			// 	}
			// 	free(all->t_cmd->cmd_tab);
			// 	free(all->t_cmd);
			// 	free(str);
			// 	if (all->shell)
			// 	{
			// 		ft_clear(&all->shell);
			// 	}
			// 	continue;
			// }
	// ft_print_triple_tab(all->t_cmd);

			//Execution
			if (ft_open_pipe(all->t_cmd) == 1)
				return (1);
			ft_exec_commande(all->t_cmd, all->t_red, &all, all->env);
			ft_waitpid(all->t_cmd);
			ft_close_pipe(all->t_cmd);

			//exit code
			// int exit_status = 0;
			ft_check_exit_status(&all);
			// Seulement mettre à jour l'exit status si un processus a réellement été exécuté
			// int process_executed = 0;
			// j = 0;
			// while (j < all->t_cmd->nbr_cmd)
			// {
			// 	if (all->t_cmd->cmd_tab[j].id1 > 0)
			// 	{
			// 		process_executed = 1;
			// 		break;
			// 	}
			// 	j++;
			// }
			// if (process_executed && WIFEXITED(all->t_cmd->status))
			// 	all->exit_status = WEXITSTATUS(all->t_cmd->status);
			// else if (process_executed == 0)
			// {
			// 	//Si aucun processus n'a été exécuté mais qu'aucune erreur n'a été détectée, exit_status = 0
			// 	int has_error = 0;
			// 	j = 0;
			// 	while (j < all->t_cmd->nbr_cmd)
			// 	{
			// 		if (all->t_cmd->cmd_tab[j].input_failed == 1
			// 			|| all->t_cmd->cmd_tab[j].output_failed == 1)
			// 		{
			// 			has_error = 1;
			// 			break;
			// 		}
			// 		j++;
			// 	}
			// 	if (has_error == 0)
			// 		all->exit_status = 0;
			// }
			// Si aucun processus n'a été exécuté à cause d'erreurs, garder l'exit_status précédent
			// printf("exit:%d\n", all->exit_status);

			//free
			ft_free_all(all);
			// j = 0;
			// while (j < all->t_cmd->nbr_cmd && all->t_cmd->cmd_tab[j].cmd_args)
			// {
			// 	ft_free_double_tab(all->t_cmd->cmd_tab[j].cmd_args);
			// 	j++;
			// }
			// free(all->t_cmd->cmd_tab);
			// free(all->t_cmd);
			// // free(all->t_red);
			// free(str);
			// if (all->shell)
			// {
			// 	ft_clear(&all->shell);
			// }
		}
		free(all);
	}
	return (0);
}
*/