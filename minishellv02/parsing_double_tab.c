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
	if (!tab)
		return (NULL);
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i + 1));
	if (!newtab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		newtab[i] = ft_strdup(tab[i]);
		if (!newtab[i])
		{
			ft_free_double_tab(newtab);
			return (NULL);
		}
		i++;
	}
	newtab[i] = 0;
	return (newtab);
}

char	**ft_add_double_tab(char *str, char **tab)
{
	int		i;
	char	**newtab;

	i = 0;
	if (!str)
		return (tab);
	if (tab == NULL)
	{
		newtab = malloc(sizeof(char *) * 2);
		if (!newtab)
			return (NULL);
		newtab[0] = ft_strdup(str);
		if (!newtab[0])
		{
			free(newtab);
			return (NULL);
		}
		newtab[1] = 0;
		return (newtab);
	}
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i + 2));
	if (!newtab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		newtab[i] = ft_strdup(tab[i]);
		if (!newtab[i])
		{
			ft_free_double_tab(newtab);
			return (NULL);
		}
		i++;
	}
	newtab[i] = ft_strdup(str);
	if (!newtab[i])
	{
		ft_free_double_tab(newtab);
		return (NULL);
	}
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
		if (!newtab)
			return (NULL);
		return (newtab[0] = 0, newtab);
	}
	while (tab[i])
		i++;
	newtab = malloc(sizeof(char *) * (i + 1));
	if (!newtab)
		return (NULL);
	i = 0;
	while (tab[i])
	{
		if (ft_strncmp(tab[i], str, ft_strlen(str)) != 0)
		{
			newtab[j] = ft_strdup(tab[i]);
			if (!newtab[j])
				return (ft_free_double_tab(newtab), NULL);
			j++;
		}
		i++;
	}
	newtab[j] = 0;
	return (ft_free_double_tab(tab), newtab);
}
