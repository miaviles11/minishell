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

#include "../../includes/minishell.h"

void signal_handler(int sign)
{
   if (sign == SIGINT)  // Ctrl+C
   {
       /* Solo salto de línea y limpio el buffer de readline */
       write(STDOUT_FILENO, "\n", 1);
       rl_on_new_line();
       rl_replace_line("", 0);
   }
	else if (sign == SIGQUIT) // 'Ctrl+\'
	{
		// En el prompt, simplemente ignoramos Ctrl+\ sin mostrar nada
		rl_redisplay();  // Solo redibujar para mantener consistencia
	}
}

void setup_signals(void)
{
    struct sigaction sa_int, sa_quit;
    
    // Configurar SIGINT (Ctrl+C)
    sa_int.sa_handler = signal_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
    
    // Configurar SIGQUIT (Ctrl+\) para ignorarlo
    sa_quit.sa_handler = SIG_IGN;  // Ignorar completamente
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
