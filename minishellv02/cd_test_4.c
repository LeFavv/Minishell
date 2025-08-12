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

void	ft_putstr(char *str);

// int ft_strcmp(char *s1, char *s2)
// {
// 	int i = 0;
	
// 	while(s1[i] && s2[i] && s1[i] == s2[i])
// 		i++;
// 	return (s1[i] - s2[i]);
// }

char **ft_replace_double_tab(char *var_name, char *str, char **tab)
{
	int		i;
	int		j;
	char	**newtab;
	char	*var_name_equal;
	
	i = 0;
	j = 0;
	if (!var_name || !str)
		return (tab);
	var_name_equal = malloc(ft_strlen(var_name) + 2);
	if (!var_name_equal)
		return (NULL);
	while (var_name[i])
	{
		var_name_equal[i] = var_name[i];
		i++;
	}
	var_name_equal[i] = '=';
	var_name_equal[i + 1] = '\0';
	if (tab == NULL)
	{
		newtab = malloc(sizeof(char *) * 1);
		if (!newtab)
			return (free(var_name_equal), NULL);
		newtab[0] = 0;
		return (free(var_name_equal), newtab);
	}
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i + 1));
	if (!newtab)
		return (free(var_name_equal), NULL);
	i = 0;
	while (tab[i])
	{
		if (ft_strncmp(tab[i], var_name, ft_strlen(var_name)) != 0)
		{
			newtab[j] = ft_strdup(tab[i]);
			if (!newtab[j])
			{
				ft_free_double_tab(newtab);
				return (free(var_name_equal), NULL);
			}
		}
		else
		{
			newtab[j] = ft_strjoin(var_name_equal, str);
			if (!newtab[j])
			{
				ft_free_double_tab(newtab);
				return (free(var_name_equal), NULL);
			}
		}
		i++;
		j++;
	}
	newtab[j] = 0;
	free(var_name_equal);
	ft_free_double_tab(tab);
	return (newtab);
}

// int is_export(char *str)
// {
// 	if (ft_strcmp(str, "export") == 0)
// 		return (1);
// 	return (0);
// }

// char *ft_strchr(char *str, char c)
// {
// 	int i = 0;

// 	if (!str || !str[0])
// 		return (NULL);

// 	while (str && str[i])
// 	{
// 		if (str[i] == c)
// 			return (&str[i]);
// 		i++;
// 	}
// 	if (c == '\0')
// 		return (&str[i]);
// 	return (NULL);
// }
/*
int is_alpha(char *str)
{
	int  i = 0;

	while (str[i])
	{
		if (!((str[i] >='a' && str[i] <= 'z' ) || (str[i] >= 'A' && str[i] <= 'Z')))
			return (0);
		i++;
	}
		return (1);
}
*/

// int is_alpha(char *str)
// {
//     int  i = 0;

//     if (!(str[i] >='a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z') && str[i] != '_')
// 		return (0);
// 	if (ft_strlen(str) == 1)
// 		return (1);
//     i++;
//     while (i < ((int)ft_strlen(str) - 1))
//     {
//         if (!((str[i] >='a' && str[i] <= 'z' ) || (str[i] >= 'A' && str[i] <= 'Z')))
//         {
//             if (!(str[i] >= '0' && str[i] <= '9'))
//             {
//                 if (str[i] != '_')
//                     return (0);
//             }
//         }
//         i++;
//     }
//     if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z') && str[i] != '_')
//         return (0);
//     return (1);
// }

// int	ft_export(char **tab, t_all **all)
// {
//     int i = 1;
//     char *var_name;
//     char *equal_pos;

//     while (tab[i])
//     {
//         equal_pos = ft_strchr(tab[i], '=');
// 		if ((!equal_pos && !is_alpha(tab[i])) || tab[i][0] == '=')
//             return (ft_err(tab[0], "not a valid identifier"), 1);
// 		if (ft_strlen(tab[i]) == 1 && tab[i][0] == '=')
// 			return (ft_err(tab[0], "not a valid identifier"), 1);
// 		else if (!equal_pos && is_alpha(tab[i]))
// 			return (0);
//         if (equal_pos)
//         {
//             var_name = ft_substr(tab[i], 0, equal_pos - tab[i]);
// 			if (!var_name)
// 				return (ft_err(tab[0], "malloc failed"), 1);
// 			if (is_alpha(var_name) == 0|| !var_name)
// 				return (ft_err(tab[0], "not a valid identifier"), free(var_name), 1);
//             if (!get_env_var(var_name, (*all)->env))
//                 (*all)->env = ft_add_double_tab(tab[i], (*all)->env);
//             else
//                 (*all)->env = ft_replace_double_tab(var_name, equal_pos + 1, (*all)->env);
//             free(var_name);
//         }
//         i++;
//     }
//     return (0);
// }

// int ft_atoi(char *str)
// {
// 	int i = 0;
// 	int sign = 1;
// 	int nb = 0;

// 	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
// 		i++;
// 	if (str[i] == '-' || str[i] =='+')
// 	{
// 		if (str[i] == '-')
// 			sign *= -1;
// 		i++;
// 	}
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		nb = nb * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	return (nb * sign);
// }

// long long ft_long_atoi(char *str)
// {
// 	long long i = 0;
// 	long long sign = 1;
// 	long long nb = 0;

// 	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
// 		i++;
// 	if (str[i] == '-' || str[i] =='+')
// 	{
// 		if (str[i] == '-')
// 			sign *= -1;
// 		i++;
// 	}
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		nb = nb * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	return (nb * sign);
// }

void ft_shlvl(t_all **all)
{
    int i = 0;
    char *equal_pos;
    int nb = 0;

    while ((*all)->env[i])
    {
        if (ft_strncmp((*all)->env[i], "SHLVL=", 6) == 0)
        {
            equal_pos = ft_strchr((*all)->env[i], '=');
            if (equal_pos && *(equal_pos + 1))
            {
                nb = ft_atoi(equal_pos + 1);
				nb++;
				char *temp = ft_itoa(nb);
                (*all)->env = ft_replace_double_tab("SHLVL", temp, (*all)->env);
                free(temp);
            }
            break;
        }
        i++;
    }
}

//return value unset = 0 a chaque fois ??
int	ft_unset(char **tab, t_all **all)
{
    int i = 1;
    while (tab[i])
    {
        if (get_env_var(tab[i], (*all)->env))
            (*all)->env = ft_remove_double_tab(tab[i], (*all)->env);
        i++;
    }
    return (0);
}

int	ft_env(t_all **all)
{
	int i = 0;
	
	while ((*all)->env[i])
	{
		ft_putstr((*all)->env[i]);
		write (1, "\n", 1);
		i++;
	}
	return (0);
}

int is_builtin_2(char **tab, t_all **all)
{
	int exit_code;

	if (is_export(tab[0]))
	{
		exit_code = ft_export(tab, all);
		(*all)->exit_status = exit_code;
		return (1); 
	}
	if (ft_strcmp(tab[0], "unset") == 0)
    {
        exit_code = ft_unset(tab, all);
        (*all)->exit_status = exit_code;
        return (1);
    }
	 if (ft_strcmp(tab[0], "env") == 0 && !tab[1])
	{
		ft_env(all);
		return (1);
	}
	return (0);
}

int is_pwd(char *str)
{
	if (ft_strcmp(str, "pwd") == 0)
		return (1);
	return (0);
}

int ft_pwd(t_all **all)
{
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)))
	{
		return (printf("%s\n", cwd), 0);
	}
	else
	{
		perror("pwd: error retrieving current directory: getcwd: cannot access parent directories: ");
		(*all)->exit_status = 1;
	}
	return (1);
}

// int is_cd(char *str)
// {
// 	if (ft_strcmp(str, "cd") == 0)
// 		return (1);
// 	return (0);
// }
// int is_home(char *str)
// {
// 	if (ft_strcmp(str, "~") == 0)
// 		return (1);
// 	return (0);
// }

// int	cd_home(t_all **all)
// {
// 	char *path = get_env_var("HOME", (*all)->env);
// 	if (!path)
// 	{
// 		perror("minishell: cd: HOME not set\n");
// 		return 1;
// 	}
// 	char cwd[PATH_MAX];
// 	getcwd(cwd, sizeof(cwd)); // sauvegarde l'ancien répertoire
// 	if (chdir(path) != 0)
// 	{
// 		perror("minishell: cd");
// 		return 1;
// 	}
// 	(*all)->env = ft_replace_double_tab("OLDPWD", cwd, (*all)->env);
// 	getcwd(cwd, sizeof(cwd));
// 	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
// 	return 0;
// }

// int cd_oldpwd(t_all **all)
// {
// 	char *oldpwd = get_env_var("OLDPWD", (*all)->env);
// 	if (!oldpwd)
// 	{
// 		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
// 		return 1;
// 	}
// 	char cwd[PATH_MAX];
// 	getcwd(cwd, sizeof(cwd));
// 	if (chdir(oldpwd) != 0)
// 	{
// 		perror("minishell: cd");
// 		return 1;
// 	}
// 	(*all)->env = ft_replace_double_tab("OLDPWD", cwd, (*all)->env);
// 	getcwd(cwd, sizeof(cwd));
// 	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
// 	ft_putstr(cwd);
// 	write(1, "\n", 1);
// 	// printf("%s\n", cwd);
// 	return 0;
// }

// int cd_root(t_all **all)
// {
// 	if (chdir("/"))
// 		return (perror("errno"), -1);
// 	getcwd("/", 1);
// 	char cwd[PATH_MAX];
// 	getcwd(cwd, sizeof(cwd));
// 	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
// 	return  (0);
// }

// int	ft_cd_change_dir(t_all **all, char *str)
// {
// 	char cwd[PATH_MAX];
// 	getcwd(cwd, sizeof(cwd)); // sauvegarde l'ancien répertoire
// 	if (chdir(str) != 0)
// 	{
// 		perror("minishell: cd");
// 		return 1;
// 	}
// 	(*all)->env = ft_replace_double_tab("OLDPWD", cwd, (*all)->env);
// 	getcwd(cwd, sizeof(cwd));
// 	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
// 	return 0;
// }

// int	homemade_cd(char **tab, t_all **all)
// {
// 	if (!is_cd(tab[0]))
// 	{
// 		// printf("1\n");
// 		return (1);
// 	}
// 	if (tab[2])
// 	{
// 		// printf("2\n");
// 		// return (perror("cd : too may arguemtents"), 1);
// 		return (ft_err(tab[0], "too many arguments"), 1);
// 	}
// 	if (is_cd(tab[0]) && (!tab[1] || is_home(tab[1])))
// 	{
// 		// printf("3\n");
// 		return (cd_home(all));
// 	}
// 	if (is_cd(tab[0]) && (ft_strcmp(tab[1], "-") == 0))
// 	{
// 		// printf("4\n");
// 		return (cd_oldpwd(all));
// 	}
// 	if (is_cd(tab[0]) && (ft_strcmp(tab[1], "/") == 0))
// 	{
// 		// printf("5\n");
// 		return (cd_root(all));	
// 	}
// 	if (is_cd(tab[0]) && tab[1])
// 	{
// 		// printf("6");
// 		return (ft_cd_change_dir(all, tab[1]));
// 	}
// 	return (1);
// }

// int    homemade_cd(char **tab, t_all **all)
// {
//     if (!is_cd(tab[0]))
//     {
//         // printf("1\n");
//         return (1);
//     }
//     if (is_cd(tab[0]) && (!tab[1] || is_home(tab[1])))
//     {
//         return (cd_home(all));
//     }
    
//     if (tab[2])
//     {
//         // printf("2\n");
//         // return (perror("cd : too may arguemtents"), 1);
//         return (ft_err(tab[0], "too many arguments"), 1);
//     }

//     if (tab[1])
//     {
//         if (is_cd(tab[0]) && (ft_strcmp(tab[1], "-") == 0))
//         {
//             // printf("4\n");
//             return (cd_oldpwd(all));
//         }
//         if (is_cd(tab[0]) && (ft_strcmp(tab[1], "/") == 0))
//         {
//             // printf("5\n");
//             return (cd_root(all));    
//         }
//         if (is_cd(tab[0]) && tab[1])
//         {
//             // printf("6");
//             return (ft_cd_change_dir(all, tab[1]));
//         }
//     }
//     return (1);
// }

// int	is_echo(char *str)
// {
// 	if (ft_strcmp(str, "echo") == 0)
// 		return (1);
// 	// if (ft_strncmp(str, "echo", 4) == 0)
// 		// return (1);
// 	return (0);
// }

// void	ft_putstr(char *str)
// {
// 	int i = 0;
// 	while (str[i])
// 	{
// 		write (1, &str[i], 1);
// 		i++;
// 	}
// }

int    is_only_n(char *str)
{
    int i = 1;

    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

// int    ft_echo(char **tab)
// {
//     int i = 1;
    
//     if (!tab[1])
//         return (write (1, "\n", 1), 0);
//     if (tab[i] && ft_strncmp(tab[i], "-n", 2) == 0)
//     {
//         while (tab[i] && tab[i][0] == '-' && is_only_n(tab[i]))
//             i++;
//         if (tab[i])
//         {
//             while (tab[i])
//             {
//                 ft_putstr(tab[i]);
//                 if (tab[i + 1])
//                     write (1, " ", 1);
//                 i++;
//             }
//             return (0);
//         }
//     }
//     else if (tab[1])
//     {
//         i = 1;
//         while (tab[i])
//         {
//             ft_putstr(tab[i]);
//             // write(1,"test",4);
//             if (tab[i + 1])
//                 write (1, " ", 1);
//             i++;
//         }
//         write (1, "\n", 1);
//     }
//     return (0);
// }

/*
int	ft_echo(char **tab)
{
	int i = 0;
	
	if (!tab[1])
		return (0);
	if (tab[1] && ft_strcmp(tab[1], "-n") == 0)
	{
		if (tab[2])
		{
			i = 2;
			while (tab[i])
			{
				ft_putstr(tab[i]);
				if (tab[i + 1])
					write (1, " ", 1);
				i++;
			}
			return (1);
		}
	}
	else if (tab[1])
	{
		i = 1;
		while (tab[i])
		{
			ft_putstr(tab[i]);
			// write(1,"test",4);
			if (tab[i + 1])
				write (1, " ", 1);
			i++;
		}
		write (1, "\n", 1);
	}
	return (0);
}
*/
int ft_is_digit(char *str)
{
	int i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int ft_exit(char **tab, t_all **all)
{
	if (!ft_is_digit(tab[1]))
	{
		write(1, "exit\n", 5); //sortie 1 ou 2 
		ft_err(tab[1], "numeric argument required");
		exit (2);
	}
	if (tab[2])
	{
		write(1, "exit\n", 5); //sortie 1 ou 2 
		ft_err(tab[1], "too many arguments");
		(*all)->exit_status = 1;
		return (1);
	}
	if ((*all)->t_cmd->nbr_cmd == 1)
		exit(ft_atoi(tab[1]) % 256);
	else if ((*all)->t_cmd->nbr_cmd > 1)
        (*all)->exit_status = ft_atoi(tab[1]) % 256;
	return (0);
}

int is_builtin_3(char **tab, t_all **all)
{
	int exit_code;

	if (is_cd(tab[0]))
	{
		// printf("cd in\n");
		exit_code = homemade_cd(tab, all);
		(*all)->exit_status = exit_code;
		// printf("exit code cd = %d\n\n", exit_code);
		// printf("exit status cd = %d\n\n", (*all)->exit_status);
		return (1);
	}
	 if (is_echo(tab[0]))
	{
		exit_code = ft_echo(tab);		
		(*all)->exit_status = exit_code;
		return (exit_code);
	}
	if (is_pwd(tab[0]))
	{
		ft_pwd(all);
		return (1);
	}
	if (ft_strcmp(tab[0], "exit") == 0)
	{
		ft_exit(tab, all);
		return (1);
		// if (tab[2] != NULL)
		// 	return (0);
		// if (!ft_is_digit(tab[1]))
		// {
		// 	write(1, "exit\n", 5); //sortie 1 ou 2 
		// 	ft_err(tab[1], "numeric argument required");
		// 	exit (2);
		// }
		// if (tab[2])
		// {
		// 	write(1, "exit\n", 5); //sortie 1 ou 2 
		// 	ft_err(tab[1], "too many arguments");
		// 	(*all)->exit_status = 1;
		// 	return (1);
		// }
		// if ((*all)->t_cmd->nbr_cmd == 1)
		// 	exit(ft_atoi(tab[1]) % 256);
		return (1);
	}
	if (is_builtin_2(tab, all))
		return (1);
	return (0);
}