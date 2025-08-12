/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafavard <vafavard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:24:01 by vafavard          #+#    #+#             */
/*   Updated: 2025/08/12 17:36:02 by vafavard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd_change_dir(t_all **all, char *str);
int	homemade_cd(char **tab, t_all **all);

int	ft_cd_change_dir(t_all **all, char *str)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, sizeof(cwd));
	if (chdir(str) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	(*all)->env = ft_replace_double_tab("OLDPWD", cwd, (*all)->env);
	getcwd(cwd, sizeof(cwd));
	(*all)->env = ft_replace_double_tab("PWD", cwd, (*all)->env);
	return (0);
}

int	homemade_cd(char **tab, t_all **all)
{
	if (!is_cd(tab[0]))
		return (1);
	if (is_cd(tab[0]) && (!tab[1] || is_home(tab[1])))
		return (cd_home(all));
	if (tab[2])
		return (ft_err(tab[0], "too many arguments"), 1);
	if (tab[1])
	{
		if (is_cd(tab[0]) && (ft_strcmp(tab[1], "-") == 0))
			return (cd_oldpwd(all));
		if (is_cd(tab[0]) && (ft_strcmp(tab[1], "/") == 0))
			return (cd_root(all));
		if (is_cd(tab[0]) && tab[1])
			return (ft_cd_change_dir(all, tab[1]));
	}
	return (1);
}
