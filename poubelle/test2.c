/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:12:08 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/20 15:12:10 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	// (void)env;

	// char *args[3];

	// args[0] = "ls -l";
	// // args[1] = "b-l";
	// args[1] = NULL;

	// if (execve("/bin/ls", args, NULL) == -1)
	// {
	// 	perror("execve erreur");
	// }
	//TEST getenv()
	//char *getenv(const char *name);
	// printenv pour les var env
	char *test = getenv("USER");
	printf("USER=%s\n", test);

	
	test = getenv("PATH");
	printf("PATH=%s\n", test);

	int i = 0;

	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	printf("%d\n", i);
}