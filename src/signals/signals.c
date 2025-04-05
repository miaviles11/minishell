/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:26 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 18:51:39 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void signal_handler(int sign)
{
	if (sign == SIGINT)  // Ctrl+C
	{
		printf("\n");
		rl_on_new_line();       // Mueve el cursor a una nueva línea
		rl_replace_line("", 0); // Borra la línea actual del buffer
		rl_redisplay();         // Redibuja el prompt
	}
	else if (sign == SIGQUIT) // 'Ctrl+\'
	{
		printf("\b\b  \b\b"); // Borra `^\` en la terminal
	}
}

void setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}
