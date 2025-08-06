/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lst_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 12:46:18 by kevwang           #+#    #+#             */
/*   Updated: 2025/07/31 12:46:18 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_size(t_list *lst);
void	ft_print(t_list *lst);
int		ft_check(char *str, char c);
void	ft_lstiter_env(t_list **lst, char **env, t_all *all);

int	ft_size(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return size;
}

void	ft_print(t_list *lst)
{
	if (lst)
	{
		int size = ft_size(lst);
		// printf("size:%d\n", size);
		int i = 0;
		while (i < size)
		{
			// printf("%d:%s:%d:%d\n", i,lst->str, lst->state, lst->redir);
			printf("%d:%s:%d:%zu\n", i,lst->str, lst->state, ft_strlen(lst->str));
			// printf("%d:'%s'\n", i,lst->str);
			lst = lst->next;
			i++;
		}
		// printf("%d\n", lst->val);
	}
}

int ft_check(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

//ft_iter pour les redirections, a modifier | pas fonctionnelle correctemnt
void	ft_lstiter_env(t_list **lst, char **env, t_all *all)
{
	t_list *temp;
	(void)env;
	(void)all;
	// char *new_str;

	temp = *lst;
	while (*lst != NULL)
	{
		// if ((*lst)->state == DOUBLEQUOTE || (*lst)->state == NORMAL)
		// {
		// 	new_str = replace_dollar_vars((*lst)->str, env, all);
		// 	if (new_str)
		// 	{
		// 		free((*lst)->str);
		// 		(*lst)->str = new_str;
		// 	}
		// }
		(*lst)->redir = -1;
		if ((*lst)->state == INPUT)
			(*lst)->next->state = INFILE;
		if ((*lst)->state == OUTPUT)
			(*lst)->next->state = OUTFILE;
		if ((*lst)->state == APPEND)
			(*lst)->next->state = OUTFILEAPPEND;
		(*lst) = (*lst)->next;
	}
	*lst = temp;
}

//pas utiliser encore
void ft_lstiteration(t_all **all, void(*f)(char *))
{
	t_list *temp;

	temp = (*all)->shell;
	while ((*all)->shell)
	{
		(*f)((*all)->shell->str);
		(*all)->shell = (*all)->shell->next;
	}
	(*all)->shell = temp;
}