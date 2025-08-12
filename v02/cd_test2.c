/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:29:24 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/02 12:22:58 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#include "minishell.h"

#define PATH_MAX	4096

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while(s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char **replace_env_var(char **env, const char *name, const char *value)
{
	int i = 0;
	int len = ft_strlen(name);
	char *new_var;
	// Supprime l'ancienne variable
	while (env && env[i])
	{
		if (ft_strncmp(env[i], (char*)name, len) == 0 && env[i][len] == '=')
		{
			free(env[i]);
			int j = i;
			while (env[j])
			{
				env[j] = env[j + 1];
				j++;
			}
			break;
		}
		i++;
	}
	new_var = ft_strjoin(name, value);
	if (!new_var)
		return env;
	env = ft_add_double_tab(new_var, env);
	return env;
}

// int is_builtin_2(char **tab)
// {
// 	  if (is_export(tab[0]))
// 	{
// 		//fonction 
// 		return (1);
// 	}
// 	 if (is_unset(tab[0]))
// 	{
// 		//fonction 
// 		return (1);
// 	}
// 	 if (is_env(tab[0]))
// 	{
// 		//fonction 
// 		return (1);
// 	}
// 	 if (is_exit(tab[0]))
// 	{
// 		//fonction 
// 		return (1);
// 	}
// 	return (0);
// }

int is_pwd(char *str)
{
	if (ft_strcmp(str, "pwd") == 0)
		return (1);
	return (0);
}

int ft_pwd(void)
{
	char cwd[PATH_MAX];
	
	getcwd(cwd, sizeof(cwd));

	printf("%s\n", cwd);
	return (0);
}

int is_cd(char *str)
{
	if (ft_strcmp(str, "cd") == 0)
		return (1);
	return (0);
}
int is_home(char *str)
{
	if (ft_strcmp(str, "/user/local") == 0)
		return (1);
	return (0);
}

// int	cd_home(t_all **all)//donner la structure pour pouvoir mettre le OLDPWD dans le tableau d'env
// {
// 	char *path = get_env_var("HOME", (*all)->env);
// 	char		*copy;	
	
// 		// path = get_env_var("HOME", (*all)->env);
// 		copy = ft_strdup(path);
// 		//faire protection
// 		if (!chdir(path))
// 			return (0);
// 		char *copy_2 = ft_strjoin("OLDPWD=", "copy");
// 		(*all)->env = ft_add_double_tab(copy_2, (*all)->env);
// 		char cwd[PATH_MAX];
// 		char *test = getcwd(cwd, sizeof(cwd));
		
// 		printf("Apres cd = %s\n", test);
// 	return (0);
// }

int	cd_home(t_all **all)
{
	char *path = get_env_var("HOME", (*all)->env);
	if (!path)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return 1;
	}
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd)); // sauvegarde l'ancien répertoire
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return 1;
	}
	// Met à jour OLDPWD dans l'environnement custom
	// (*all)->env = replace_env_var((*all)->env, "OLDPWD", cwd);
	getcwd(cwd, sizeof(cwd));
	printf("Apres cd = %s\n", cwd);
	return 0;
}

int cd_oldpwd(t_all **all)
{
	char *oldpwd = get_env_var("OLDPWD", (*all)->env);
	if (!oldpwd)
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		return 1;
	}
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd)); // sauvegarde l'ancien répertoire
	if (chdir(oldpwd) != 0)
	{
		perror("minishell: cd");
		return 1;
	}
	// Met à jour OLDPWD
	(*all)->env = replace_env_var((*all)->env, "OLDPWD", cwd);
	getcwd(cwd, sizeof(cwd));
	printf("Apres cd - = %s\n", cwd);
	return 0;
}

// int cd_oldpwd(void)
// {
// 	char	cwd[PATH_MAX];
// 	const char *path;
	
// 	path = getenv("OLDPWD");
// 	if (chdir(path))
// 		printf("%s\n", strerror(errno));
// 	getcwd(cwd, sizeof(cwd));
// 	printf("%s", cwd);
// 	return (0);
// }

int cd_root(void)
{
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	// const char *path;
	
	if (pid == 0) //processus enfant
	{
		close(fd[0]);
		if (chdir("/"))
		printf("%s\n", strerror(errno));
		getcwd("/", 1);
		write(fd[1], "/", 1);
		close(fd[1]);
	}
	else //processus parent
	{
		close(fd[1]);
		wait(NULL);
		char str[PATH_MAX];
		int i = read(fd[0], str, PATH_MAX);
		str[i] = '\0';
		if (!chdir(str))
			return (close(fd[0]), 0);
		return (close(fd[0]), 1);
	}
	return (0);
}

// /!\ CHANGER OLDPWD A CHAQUE UTILISATIION DE CD
// ATTENDRE IMPLEMENTATION EXPORT
int	homemade_cd(char **tab, t_all **all)
{
	if (!is_cd(tab[0]))
	{
		printf("1\n");
		return (1);
	}
	if (tab[2])
	{
		printf("2\n");
		return (perror("cd : too may arguemtents"), 1);
	}
	if (is_cd(tab[0]) && (!tab[1] || is_home(tab[1])))
	{
		printf("3\n");
		return (cd_home(all)); //passer cd_home de type int
	}
	if (is_cd(tab[0]) && (ft_strcmp(tab[1], "-") == 0)) // cd - /!\ FAIRE APPEL A LE OLDPWD
	{
		printf("4\n");
		return (cd_oldpwd(all));
	}
	if (is_cd(tab[0]) && (ft_strcmp(tab[1], "\\") == 0))
	{
		printf("5\n");
		return (cd_root());	
	}
	//if (is_cd(tab[0]) )
	return (0);
}

int is_builtin2(char **tab, t_all **all)
{
	if (is_cd(tab[0]))
	{
		printf("cd in\n");
		homemade_cd(tab, all);
		return (1);
	}
	//  if (is_echo(tab[0]))
	// {
	// 	//fonction 
	// 	return (1);
	// }
	 if (is_pwd(tab[0]))
	{
		ft_pwd(); 
		return (1);
	}
	// if (is_builtin_2(tab))
	// 	return (1);
	return (0);
}

// int main()
// {
// 	char s[100];
// 	char *args[3];

// 	args[0] = "cd";
// 	args[1] = "-";
// 	args[2] = NULL;

// 	char *args2[2];

// 	args2[0] = "pwd";
// 	// args2[1] = "-";
// 	args2[1] = NULL;
// 	printf("je suis la%s\n", getcwd(s, 100)); 
// 	is_builtin(args);
// 	printf("je suis ou%s\n", getcwd(s, 100)); 
// 	is_builtin(args2);
// 	// chdir("..");
// }

// int main(int argc, char **argv)
// {
// 	const char *path;
// 	if (argc >= 2)
// 	{
// 		if (argc == 2)
// 		{
// 			int fd[2];
// 			pipe(fd);
// 			char cwd[PATH_MAX];
// 			pid_t pid = fork();
		
// 			if (is_cd(argv[1]))
// 			{
// 				if (pid == 0)
// 				{
// 					close(fd[0]);
// 					path = getenv("HOME");
// 					if (chdir(path))
// 						printf("%s\n", strerror(errno));
// 					printf("path = %s\n", path);
// 					if (getcwd(cwd, sizeof(cwd)) != NULL)
// 						printf("Child : repertoire courant apres chdir %s\n", cwd);
// 					else
// 						perror("getcwd child");
// 					write(fd[1], cwd, strlen(cwd));
// 					close(fd[1]);
// 				}
// 				else
// 				{
// 					close(fd[1]);
// 				wait(NULL);
					
// 					if (getcwd(cwd, sizeof(cwd)) != NULL)
// 						printf("Parent : repertoire courant apres fork %s\n", cwd);
// 					char str[PATH_MAX];
// 					int i = read(fd[0], str, PATH_MAX);
// 					str[i] = '\0';
// 					printf("ce que je read : %s\n", str);
// 					chdir(str);
// 					if (getcwd(cwd, sizeof(cwd)) != NULL)
// 						printf("Parent : repertoire courant apres chdir %s\n", cwd);
// 					close(fd[0]);
					
// 					// else
// 					// 	perror("getcwd parent");
// 				}
// 			}
// 			else
// 				return (1);
			
// 		}
// 	}
//}
