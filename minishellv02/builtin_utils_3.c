/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 08:04:35 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/14 08:12:19 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_shlvl(t_all **all);
int		is_only_n(char *str);
int		ft_is_digit(char *str);
int		ft_exit(char **tab, t_all **all);

void	ft_shlvl(t_all **all)
{
	int		i;
	int		nb;
	char	*equal_pos;
	char	*temp;

	i = 0;
	nb = 0;
	while ((*all)->env[i])
	{
		if (ft_strncmp((*all)->env[i], "SHLVL=", 6) == 0)
		{
			equal_pos = ft_strchr((*all)->env[i], '=');
			if (equal_pos && *(equal_pos + 1))
			{
				nb = ft_atoi(equal_pos + 1);
				nb++;
				temp = ft_itoa(nb);
				(*all)->env = ft_replace_double_tab("SHLVL", temp, (*all)->env);
				free(temp);
			}
			break ;
		}
		i++;
	}
}

int	is_only_n(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_is_digit(char *str)
{
	int	i;

	i = 1;
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

int	ft_exit(char **tab, t_all **all)
{
	if (!ft_is_digit(tab[1]))
	{
		write(1, "exit\n", 5);
		ft_err(tab[1], "numeric argument required");
		exit (2);
	}
	if (tab[2])
	{
		write(1, "exit\n", 5);
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
