/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 12:20:41 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/06 12:20:41 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

int	ft_strncmp(char *s1, char *s2, unsigned int n)
{	
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && i < n - 1 && s1[i] == s2[i])
	{
			i++;
	}
	return (s1[i] - s2[i]);
}

void	ft_print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

char	*ft_strdup(char *s)
{
	char	*newstr;
	int		i;

	i = 0;
	newstr = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (newstr == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		newstr[i] = s[i];
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*newstr;
	size_t			s_len;

	i = 0;
	s_len = ft_strlen(s);
	if (start > s_len)
	{
		return (ft_strdup(""));
	}
	if (s_len - start > len)
		newstr = malloc(sizeof(char) * len + 1);
	else
		newstr = malloc(sizeof(char) * (s_len - start) + 1);
	if (newstr == NULL)
		return (NULL);
	while (i < len && s[start + i] != '\0'
		&& (size_t)start < ft_strlen(s))
	{
		newstr[i] = s[start + i];
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}
