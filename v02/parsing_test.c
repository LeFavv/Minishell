/* ************************************************************************** */
/*                                                                            */
/*			//Compte le nombre de commande
			all->t_cmd = malloc(sizeof(t_commande));
			all->t_cmd->nbr_cmd = ft_count_commands(all->shell);
			// printf("nbr commande: %d\n", all->t_cmd->nbr_cmd);                                                    :::      ::::::::   */
/*   parsing_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:38:30 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/18 11:38:32 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
//execute les commandes 
int ft_exec_commande(t_commande *t_cmd, t_redir *t_red, t_all *all, char **env)
{
	int i;
	(void)t_red;
	(void)all;
	i = 0;
	while (i < t_cmd->nbr_cmd)
	{
		// Vérifier si la commande a des redirections d'entrée qui ont échoué
		if (t_cmd->cmd_tab[i].input_failed == 1 || t_cmd->cmd_tab[i].output_failed == 1)
		{
			if (t_cmd->cmd_tab[i].input_failed == 1)
			{
				perror(t_cmd->cmd_tab[i].in_str);
			}
			if (t_cmd->cmd_tab[i].output_failed == 1)
			{
				perror(t_cmd->cmd_tab[i].out_str);
			}
			// Ne pas exécuter la commande si redirection d'entrée a échoué
			t_cmd->cmd_tab[i].id1 = -1; // Marquer comme non créé
			i++;
			continue;
		}

		//condition pour verifier uniquement si les builtin existent
		//puis dup2
		//puis executer les fonctions
		if (is_builtin2(t_cmd->cmd_tab[i].cmd_args, &all) == 1)
		{
			printf("builtin\n");
			t_cmd->cmd_tab[i].id1 = -1; // Les builtins n'ont pas de processus fils
		}
		else
		{
			t_cmd->cmd_tab[i].id1 = fork();
			if (t_cmd->cmd_tab[i].id1 == 0)
			{
				// Gestion des redirections d'entrée pour toutes les commandes
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
				if (exec(t_cmd->cmd_tab[i].cmd_args, env) == -1)
					exit(127);
				exit(1);
			}
		}
		i++;
	}
	ft_close_pipe(t_cmd);
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
		{
			first_cmd = 1;
		}
		else if ((*lst)->state == NORMAL || (*lst)->state == DOUBLEQUOTE || (*lst)->state == SINGLEQUOTE)
		{
			if (first_cmd)
			{
				(*lst)->state = CMD;
				first_cmd = 0;
			}
			else
			{
				(*lst)->state = ARG;
			}
		}
		*lst = (*lst)->next;
	}
	*lst = temp;
}

//caca parsing_test.c pipex_path.c parsing_dollar.c minishell_utils.c ft_strjoin.c ft_split.c ft_itoa.c -lreadline -o minishell
int main(int argc, char **argv, char **env)
{
	(void)argv;
	t_all *all;
	char *str;

	if (argc == 1)
	{
		all = malloc(sizeof(t_all));
		all->env = env;
		all->exit_status = 0; // Initialiser l'exit status à 0 au début du programme
		while (1)
		{
			all->shell = NULL;
			// Ne pas réinitialiser exit_status ici, le garder de la commande précédente
			str = readline("CacaTest >");
			add_history(str);

			if (ft_strncmp(str, "exit", 4) == 0)
			{
				free(str);
				exit(0);
			}
			//Parse_decoupe bah elle decoupe l'input en liste chaine
			if (ft_parse_decoupe(str, &all->shell, all) == -1)
			{
				// Skip this iteration if parsing failed due to unclosed quotes
				free(str);
				continue;
			}
			
			//lstiter_env pour verifier les redirecions '<' '>' '>>' '<<'
			ft_lstiter_env(&all->shell, env, all);
	// ft_print(all->shell);

			// ft_assign_cmd_arg_states(&all->shell);


			//Compte le nombre de commande
			all->t_cmd = malloc(sizeof(t_commande));
			all->t_cmd->nbr_cmd = ft_count_commands(all->shell);
			// printf("nbr commande: %d\n", t_cmd->nbr_cmd);

			//Creation des redirections
			// all->t_red = malloc(sizeof(t_redir));
			// all->t_red->infd = -1;
			// all->t_red->outfd = -1;
			// ft_create_fd(&all->shell, &all->t_red);


			all->t_cmd->cmd_tab = malloc(sizeof(t_cmd_tab) * all->t_cmd->nbr_cmd);
			if (!all->t_cmd->cmd_tab)
				return (1);
			// all->t_cmd->cm
			//Creation des doubles tableaux pour les commandes
			ft_set_triple_tab_null(all->t_cmd);
			if (ft_create_triple_tab(&all->shell, &all->t_cmd, &all) == -1)
			{
				free(str);
				continue;
			}
	// ft_print_triple_tab(all->t_cmd);

			//Execution
			ft_open_pipe(all->t_cmd);
			ft_exec_commande(all->t_cmd, all->t_red, all,env);
			ft_waitpid(all->t_cmd);
			ft_close_pipe(all->t_cmd);

			//exit code
			// int exit_status = 0;
			// Seulement mettre à jour l'exit status si un processus a réellement été exécuté
			int process_executed = 0;
			int j = 0;
			while (j < all->t_cmd->nbr_cmd)
			{
				if (all->t_cmd->cmd_tab[j].id1 > 0)
				{
					process_executed = 1;
					break;
				}
				j++;
			}
			
			if (process_executed && WIFEXITED(all->t_cmd->status))
			{
				all->exit_status = WEXITSTATUS(all->t_cmd->status);
			}
			else if (process_executed == 0)
			{
				// Si aucun processus n'a été exécuté mais qu'aucune erreur n'a été détectée, exit_status = 0
				int has_error = 0;
				j = 0;
				while (j < all->t_cmd->nbr_cmd)
				{
					if (all->t_cmd->cmd_tab[j].input_failed == 1
						|| all->t_cmd->cmd_tab[j].output_failed == 1)
					{
						has_error = 1;
						break;
					}
					j++;
				}
				if (has_error == 0)
					all->exit_status = 0;
			}
			// Si aucun processus n'a été exécuté à cause d'erreurs, garder l'exit_status précédent
			// printf("exit:%d\n", all->exit_status);

			//free
			j = 0;
			while (j < all->t_cmd->nbr_cmd && all->t_cmd->cmd_tab[j].cmd_args)
			{
				ft_free_double_tab(all->t_cmd->cmd_tab[j].cmd_args);
				j++;
			}
			free(all->t_cmd->cmd_tab);
			free(all->t_cmd);
			free(all->t_red);
			free(str);
			if (all->shell)
			{
				ft_clear(&all->shell);
			}
		}
		free(all);
	}
	return (0);
}
