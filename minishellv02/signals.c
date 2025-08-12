/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevwang <kevwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:56:26 by kevwang           #+#    #+#             */
/*   Updated: 2025/08/11 12:56:28 by kevwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void set_exit(int *exit2)
{
	static int *new_exit = NULL;

	if (new_exit == NULL)
		new_exit = exit2;

	*new_exit = 130;
}

void ft_test(int signum)
{
	(void)signum;
	g_sigvaleur = 1; // IMPORTANT : Marquer qu'un signal sigint a été reçu
	write(1, "\n", 1);
    rl_replace_line("", 0);      // Vider la ligne courante
    rl_on_new_line();            // Indiquer qu'on est sur une nouvelle ligne
    rl_redisplay();              // Réafficher le prompt
    set_exit(&g_sigvaleur);
}

void ft_sigquit(int signum)
{
	(void)signum;
	// Pour SIGQUIT (Ctrl+\) au prompt : ne rien faire (comme bash)
	// Le signal sera géré par les processus enfants avec SIG_DFL
}
