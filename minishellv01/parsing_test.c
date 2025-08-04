/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 11:38:30 by kevwang           #+#    #+#             */
/*   Updated: 2025/08/04 17:42:55 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define PATH_MAX	4096


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
		if (is_builtin(t_cmd->cmd_tab[i].cmd_args, &all) == 1)
		{
			// printf("builtin\n");
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

char **create_default_env(void)
{
    char **tab = NULL;
    char    *str = NULL;

    char    cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    str = ft_strjoin("PWD=", cwd); // non free
    tab = ft_add_double_tab(str, tab);
    tab = ft_add_double_tab("_=/usr/bin/env", tab);
	tab = ft_add_double_tab("SHLVL=1", tab);
    return (tab);
}

//caca parsing_test.c pipex_path.c parsing_dollar.c minishell_utils.c ft_strjoin.c ft_split.c ft_itoa.c -lreadline -o minishell
int main(int argc, char **argv, char **env)
{
	(void)argv;
	t_all *all;
	char *str;

	// ft_shlvl(&all);
	printf("salut minishell\n");
	if (argc == 1)
	{
		all = malloc(sizeof(t_all));
		// all->env = env;
		if (env[0])
		{
        	all->env = env;
			ft_shlvl(&all);
		}
    	else
    	{
   	    	// printf("je suis ici\n");
        	all->env = create_default_env();			
   	 	}
		while (1)
		{
			all->shell = NULL;
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
			all->t_red = malloc(sizeof(t_redir));
			all->t_red->infd = -1;
			all->t_red->outfd = -1;
			ft_create_fd(&all->shell, &all->t_red);


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
			// ft_exec_commande(all->t_cmd, all->t_red, all,env);
			ft_exec_commande(all->t_cmd, all->t_red, all,all->env);
			ft_waitpid(all->t_cmd);
			ft_close_pipe(all->t_cmd);

			//exit code
			// int exit_status = 0;
			if (WIFEXITED(all->t_cmd->status))
				all->exit_status = WEXITSTATUS(all->t_cmd->status);
			// printf("exit:%d\n", all->exit_status);

			//free
			int j = 0;
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
