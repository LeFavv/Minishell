/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 07:05:17 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/14 07:56:15 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_replace_double_tab(char *var_name, char *str, char **tab);
int		find_char_tab_size(char **tab);
char	**new_tab_init(char **tab, char *str, char *v_n, char *v_n_e);
char	*var_name_equal_init(char *var_name);

char	**ft_replace_double_tab(char *var_name, char *str, char **tab)
{
	char	**newtab;
	char	*var_name_equal;

	if (!var_name || !str)
		return (tab);
	var_name_equal = var_name_equal_init(var_name);
	if (!var_name_equal)
		return (NULL);
	if (tab == NULL)
	{
		newtab = malloc(sizeof(char *) * 1);
		if (!newtab)
			return (free(var_name_equal), NULL);
		newtab[0] = 0;
		return (free(var_name_equal), newtab);
	}
	newtab = new_tab_init(tab, str, var_name, var_name_equal);
	if (!newtab)
		return (free(var_name), NULL);
	ft_free_double_tab(tab);
	return (free(var_name_equal), newtab);
}

int	find_char_tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**new_tab_init(char **tab, char *str, char *v_n, char *v_n_e)
{
	int		i;
	char	**newtab;

	i = 0;
	newtab = malloc(sizeof(char *) * (find_char_tab_size(tab) + 1));
	if (!newtab)
		return (free(v_n_e), NULL);
	while (tab[i])
	{
		if (ft_strncmp(tab[i], v_n, ft_strlen(v_n)) != 0)
		{
			newtab[i] = ft_strdup(tab[i]);
			if (!newtab[i])
				return (NULL);
		}
		else
		{
			newtab[i] = ft_strjoin(v_n_e, str);
			if (!newtab[i])
				return (NULL);
		}
		i++;
	}
	newtab[i] = 0;
	return (newtab);
}

char	*var_name_equal_init(char *var_name)
{
	char	*var_name_equal;
	int		i;

	i = 0;
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
	return (var_name_equal);
}
