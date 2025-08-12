/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:37:08 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/12 17:53:38 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_echo(char *str)
{
	if (ft_strcmp(str, "echo") == 0)
		return (1);
	return (0);
}

int	ft_echo_option_n(char **tab, int i)
{
	while (tab[i] && tab[i][0] == '-' && is_only_n(tab[i]))
		i++;
	if (tab[i])
	{
		while (tab[i])
		{
			ft_putstr(tab[i]);
			if (tab[i + 1])
				write (1, " ", 1);
			i++;
		}
		return (0);
	}
	return (1);
}

int	ft_echo(char **tab)
{
	int	i;

	i = 1;
	if (!tab[1])
		return (write (1, "\n", 1), 0);
	if (tab[i] && ft_strncmp(tab[i], "-n", 2) == 0)
	{
		if (!ft_echo_option_n(tab, i))
			return (0);
	}
	else if (tab[1])
	{
		i = 1;
		while (tab[i])
		{
			ft_putstr(tab[i]);
			if (tab[i + 1])
				write (1, " ", 1);
			i++;
		}
		write (1, "\n", 1);
	}
	return (0);
}
