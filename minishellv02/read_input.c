/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:27:19 by kevwang           #+#    #+#             */
/*   Updated: 2025/08/11 13:27:20 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_read_input(t_all **all)
{
	(*all)->str = readline("CacaTest > ");
	
	if (!(*all)->str) // Ctrl+D (EOF)
	{
		write(1, "exit\n", 5);
		return (-1);
	}
	if (!(*all)->str[0]) // Chaîne vide
	{
		free((*all)->str);
		return (-2);
	}

	// Si on arrive ici, une commande valide a été saisie
	// Reset g_sigvaleur car nous avons une vraie commande à traiter
	g_sigvaleur = 0;
	
	add_history((*all)->str);
	return (0);
}