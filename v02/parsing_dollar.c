/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:58:42 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/26 14:58:44 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strcpy(char *dest, char *src);
int ft_isalnum(char c);
char *get_env_var(char *str, char **env);
char *get_env_name(char *str, int start);
char *replace_dollar_vars(char *str, char **env, t_all *all);


char *ft_strcpy(char *dest, char *src)
{
	int i;
	
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int ft_isalnum(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

char *get_env_var(char *str, char **env)
{
	int i;
	int len;

	i = 0;
	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	while (env[i])
	{
		if (ft_strncmp(str, env[i], len) == 0 && env[i][len] == '=')
		{
			return (&env[i][len+1]);
		}
		i++;
	}
	return (NULL);
}

char *get_env_name(char *str, int start)
{
	int i;
	int len;

	len = 0;
	i = start;
	// printf("test1:%s\n", str);
	if (!str)
		return (NULL);
	// printf("test2:%s\n", str);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		i++;
		len++;
	}
	if (len == 0)
		return (NULL);
	return (ft_substr(str, start, len));
}

//Deuxieme etape du parsing
//expand les $ sauf dans les singlequote
//enleve les quotes non literal
//recupere le exit status avec $?
char *replace_dollar_test2(char *str, char **env, t_all *all)
{
    int i;
    char *result;
    char *env_name;
    char *env_var;
    char *temp;
    int len;
    int indoublequote;
    int insinglequote;

    indoublequote = 0;
    insinglequote = 0;
    result = malloc(sizeof(char) * 1);
    if (!result)
        return (NULL);
    result[0] = '\0';

    i = 0;
    while (str[i])
    {   
        if (str[i] == '"' && !insinglequote)
        {
            indoublequote = !indoublequote;
            // Ajouter le guillemet au résultat
            // len = ft_strlen(result);
            // temp = malloc(sizeof(char) * (len + 2));
            // if (!temp)
            //     return (NULL);
            // ft_strcpy(temp, result);
            // temp[len] = str[i];
            // temp[len + 1] = '\0';
            // free(result);
            // result = temp;
            i++;
        }
        else if (str[i] == '\'' && !indoublequote)
        {
            insinglequote = !insinglequote;
            // Ajouter le guillemet au résultat
            // len = ft_strlen(result);
            // temp = malloc(sizeof(char) * (len + 2));
            // if (!temp)
            //     return (NULL);
            // ft_strcpy(temp, result);
            // temp[len] = str[i];
            // temp[len + 1] = '\0';
            // free(result);
            // result = temp;
            i++;
        }
        else if (str[i] == '\\' && !insinglequote)
        {
            char next = str[i + 1];
            
            if (indoublequote)
            {
                if (next == '"' || next == '\\' || next == '$' || next == '`')
                {
                    // Échapper le caractère suivant
                    len = ft_strlen(result);
                    temp = malloc(sizeof(char) * (len + 2));
                    if (!temp)
                        return (NULL);
                    ft_strcpy(temp, result);
                    temp[len] = next;
                    temp[len + 1] = '\0';
                    free(result);
                    result = temp;
                    i += 2;
                }
                else
                {
                    len = ft_strlen(result);
                    temp = malloc(sizeof(char) * (len + 2));
                    if (!temp)
                        return (NULL);
                    ft_strcpy(temp, result);
                    temp[len] = str[i];
                    temp[len + 1] = '\0';
                    free(result);
                    result = temp;
                    i++;
                }
            }
            else
            {
                if (next != '\0')
                {
                    len = ft_strlen(result);
                    temp = malloc(sizeof(char) * (len + 2));
                    if (!temp)
                        return (NULL);
                    ft_strcpy(temp, result);
                    temp[len] = next;
                    temp[len + 1] = '\0';
                    free(result);
                    result = temp;
                    i += 2;
                }
                else
                {
                    len = ft_strlen(result);
                    temp = malloc(sizeof(char) * (len + 2));
                    if (!temp)
                        return (NULL);
                    ft_strcpy(temp, result);
                    temp[len] = str[i];
                    temp[len + 1] = '\0';
                    free(result);
                    result = temp;
                    i++;
                }
            }
        }
        else if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '"')
            && (insinglequote == 0 && indoublequote == 0))
        {
            i++;
        }
        else if (str[i] == '$' && str[i + 1] != '\0' && insinglequote == 0)
        {
            if (str[i + 1] == '?')
            {
                all->exit_status_char = ft_itoa(all->exit_status);
                temp = ft_strjoin(result, all->exit_status_char);
                free(result);
                result = temp;
                free(all->exit_status_char);
                i += 2;
            }
            else
            {
                env_name = get_env_name(str, i + 1);
                if (env_name)
                {
                    env_var = get_env_var(env_name, env);
                    if (env_var)
                    {
                        temp = ft_strjoin(result, env_var);
                        free(result);
                        result = temp;
                    }
                    i = i + ft_strlen(env_name) + 1;
                    free(env_name);
                }
                else
                {
                    len = ft_strlen(result);
                    temp = malloc(sizeof(char) * (len + 2));
                    if (!temp)
                        return (NULL);
                    ft_strcpy(temp, result);
                    temp[len] = str[i];
                    temp[len + 1] = '\0';
                    free(result);
                    result = temp;
                    i++;
                }
            }
        }
        else
        {
            len = ft_strlen(result);
            temp = malloc(sizeof(char) * (len + 2));
            if (!temp)
                return (NULL);
            ft_strcpy(temp, result);
            temp[len] = str[i];
            temp[len + 1] = '\0';
            free(result);
            result = temp;
            i++;
        }
    }
    return (result);
}

/*
char *replace_dollar_vars(char *str, char **env, t_all *all)
{
	(void)env;
	(void)all;
	int i;
	char *result;
	char *env_name;
	char *env_var;
	char *temp;
	int len;

	result = malloc(sizeof(char) * 1);
	if (!result)
		return (NULL);
	result[0] = '\0';

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			if (str[i + 1] == '?')
			{
				all->exit_status_char = ft_itoa(all->exit_status);
				temp = ft_strjoin(result, all->exit_status_char);
				free(result);
				result = temp;
				free(all->exit_status_char);
				i += 2;
			}
			else
			{
				env_name = get_env_name(str, i + 1);
				if (env_name)
				{
					env_var = get_env_var(env_name, env);
					if (env_var)
					{
						temp = ft_strjoin(result, env_var);
						free(result);
						result = temp;
					}
					i = i + ft_strlen(env_name) + 1;
					free(env_name);
				}
				else
				{
					len = ft_strlen(result);
					temp = malloc(sizeof(char) * (len + 2));
					if (!temp)
						return (NULL);
					ft_strcpy(temp, result);
					temp[len] = str[i];
					temp[len + 1] = '\0';
					free(result);
					result = temp;
					i++;
				}
			}
		}
		else
		{
			len = ft_strlen(result);
			temp = malloc(sizeof(char) * (len + 2));
			if (!temp)
				return (NULL);
			ft_strcpy(temp, result);
			temp[len] = str[i];
			temp[len + 1] = '\0';
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}
*/


/*
int main(int argc, char **argv, char **env)
{
	char *str;

	(void)env;
	(void)argv;
	if (argc == 1)
	{	
		// str = argv[1];
		char *r1 = readline("Input >");

		str = replace_dollar_test2(r1, env);

		// int i = 0;
		// while (argv[1][i])
		// {
		// 	if (str[i] == '$' && str[i+1] != '\0')
		// 	{
		// 		str = get_env_name(argv[1], i + 1);
		// 		printf("%s\n", str);
		// 	}
		// 	i++;
		// }
		


		// str = get_env_name(r1, 0);
		printf("%s\n", str);
	}
	return (0);
}*/
