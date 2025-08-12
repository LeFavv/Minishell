/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_test.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:29:24 by vafavard          #+#    #+#             */
/*   Updated: 2025/07/30 15:38:50 by vafavard         ###   ########.fr       */
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

// size_t	ft_strlen(const char *str)
// {
// 	size_t	i;

// 	i = 0;
// 	while (str[i] != '\0')
// 	{
// 		i++;
// 	}
// 	return (i);
// }

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while(s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
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
	// int i = 0;

	// while (str[i])
	// {
	//     if (str[0] == 'p'
	//         && str[1] == 'w'
	//             && str[2] == 'd'
	//                 && !str[3])
	//         return (1);
	//     return (0);
	// }
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
	// char *compare;
	// int i = 0;
	
	// compare = malloc(sizeof(char) * 12);//faire juste un strcomp
	// if (!compare)
	// 	return (0);
	// compare = "/user/local";
	// compare[12] = '\0';
	// if (str[0] == '~' && !str[1])
	// 	return (free(compare), 1);
	// if (ft_strlen(compare) != ft_strlen(str))
	// 	return (free(compare), 0);
	// while (compare[i])
	// {
	// 	if (compare[i] == str[i])
	// 		i++;
	// 	break;
	// }
	// if (compare[i] == '\0')
	// 	return (free(compare), 1);
	// else
	// 	return (free(compare), 0);
	if (ft_strcmp(str, "/user/local") == 0)
		return (1);
	return (0);
	
}

int	cd_home(void)
{
	int fd[2];
	pipe(fd);
	char cwd[PATH_MAX];
	pid_t pid = fork();
	const char *path;
		
	if (pid == 0)
	{
		close(fd[0]);
		path = getenv("HOME");
		if (!chdir(path))
			return (0);
			// printf("%s\n", strerror(errno));
		if (getcwd(cwd, sizeof(cwd)) == NULL)
			perror("getcwd child");
		write(fd[1], cwd, strlen(cwd));
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		wait(NULL);
		char str[PATH_MAX];
		int i = read(fd[0], str, PATH_MAX);
		str[i] = '\0';
		if (!chdir(str))
			return (close(fd[0]), 1);
		close(fd[0]);
	}	
	return (0);
}

int cd_oldpwd(void)
{
	int fd[2];
	pipe(fd);
	char	cwd[PATH_MAX];
	pid_t pid = fork();
	const char *path;
	
	if (pid == 0) //processus enfant
	{
		close(fd[0]);
		path = getenv("OLDPWD");
		printf("path:%s\n", path);
		if (chdir(path))
			printf("%s\n", strerror(errno));
		getcwd(cwd, sizeof(cwd));
		write(fd[1], cwd, ft_strlen(cwd));
		close(fd[1]);
	}
	else //processus parent
	{
		printf("paret\n");
		close(fd[1]);
		wait(NULL);
		char str[PATH_MAX];
		int i = read(fd[0], str, PATH_MAX);
		str[i] = '\0';
		if (chdir(str))
			return (close(fd[0]), 0);
		printf("parent str:%s\n", str);
		return (close(fd[0]), 1);
	}
	return (0);
}

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
int	homemade_cd(char **tab)
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
		return (cd_home()); //passer cd_home de type int
	}
	if (is_cd(tab[0]) && (ft_strcmp(tab[1], "-") == 0)) // cd - /!\ FAIRE APPEL A LE OLDPWD
	{
		printf("4\n");
		return (cd_oldpwd());
	}
	if (is_cd(tab[0]) && (ft_strcmp(tab[1], "\\") == 0))
	{
		printf("5\n");
		return (cd_root());	
	}
	//if (is_cd(tab[0]) )
	return (0);
}

int is_builtin(char **tab)
{
	if (is_cd(tab[0]))
	{
		printf("cd in\n");
		homemade_cd(tab);
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
