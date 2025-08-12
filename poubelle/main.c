/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:49:06 by kevwang           #+#    #+#             */
/*   Updated: 2025/06/26 13:11:48 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_count_pipe(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count++;
		i++;
	}
	return (count);
}

//jsp si c ce quil faut
//ex: str="/bin/ls" return "ls"
char *ft_str_last(char *str)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (str[i])
		i++;
	while (str[i-j] != '/' && j < i)
		j++;
	if (str[i-j] == '/')
		j--;
	return (&str[i-j]);
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;

	//TEST readline()
	char *Ptest;
	char *r1;
	char *r2;
	int id1;
	int nbr_pipe;
	// int loop = 1;
	while(1)
	{
		Ptest = "caca@Minishell > ";
		r1 = readline(Ptest);

		add_history(r1);

		if (ft_strncmp(r1, "exit", 4) == 0)
		{
			free(r1);  // Libérer la mémoire avant de sortir
			// rl_clear_history();
			// break ;
			exit(1);
		}
		r2 = ft_str_last(r1);
		nbr_pipe = ft_count_pipe(r1);
		// printf("%s\n", r1);
		id1 = fork();
		if (id1 == 0)
		{
			if (ft_strncmp(r1, "history -c", 11) == 0)
					rl_clear_history();
			else
				exec_v2(r2, env);
			// id2 = fork()
		}
		else
		{
			wait(NULL);
		}
		free(r1);
		// printf("testtoto\n");
	}

/*
	//TEST getcwd()
	//obtenir le lien absolu actuel
	char *pwd;
	pwd = getcwd(NULL, 0);
	printf("pwd: %s\n", pwd);

	//TEST chdir()
	//changer le lien aboslue actuel
	pwd = getcwd(NULL, 0);
	printf("pwd AVANT chdir: %s\n", pwd);

	chdir("/home/kevwang/Desktop/gitenutrofbg/cercle4/minishell/minishellv01/dossiertest");

	pwd = getcwd(NULL, 0);
	printf("pwd APRES chdir: %s\n", pwd);
*/
}
