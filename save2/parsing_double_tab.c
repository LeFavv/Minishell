/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_double_tab.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:30:21 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:30:21 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_copy_double_tab(char **tab);
char	**ft_add_double_tab(char *str, char **tab);

char	**ft_copy_double_tab(char **tab)
{
	int		i;
	char	**newtab;

	i = 0;
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tab[i])
	{
		newtab[i] = ft_strdup(tab[i]);
		i++;
	}
	newtab[i] = 0;
	return (newtab);
}

char **ft_add_double_tab(char *str, char **tab)
{
	int		i;
	char	**newtab;

	i = 0;
	if (tab == NULL)
	{
		newtab = malloc(sizeof(char *) * 2);
		newtab[0] = ft_strdup(str);
		newtab[1] = 0;
		return (newtab);
	}
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (tab[i])
	{
		newtab[i] = ft_strdup(tab[i]);
		i++;
	}
	newtab[i] = ft_strdup(str);
	i++;
	newtab[i] = 0;
	ft_free_double_tab(tab);
	return (newtab);
}

char **ft_remove_double_tab(char *str, char **tab)
{
	int		i;
	int		j;
	char	**newtab;

	i = 0;
	j = 0;
	if (tab == NULL)
	{
		newtab = malloc(sizeof(char *) * 1);
		// newtab[0] = ft_strdup(str);
		newtab[0] = 0;
		return (newtab);
	}
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i));
	i = 0;
	while (tab[i])
	{
		if (ft_strncmp(tab[i], str, ft_strlen(str)) != 0)
			newtab[i] = ft_strdup(tab[i]);
		i++;
	}
	// newtab[i] = ft_strdup(str);
	// i++;
	newtab[i] = 0;
	ft_free_double_tab(tab);
	return (newtab);
}