/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:29:26 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/20 19:34:39 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Indicador de que el shell está en modo espera de prompt */
extern volatile sig_atomic_t g_interactive;

/* Manejador de señales para el proceso padre (prompt interactivo) */
void handle_parent_signal(int sign)
{
    if (sign == SIGINT)  // Ctrl+C
    {
        write(STDOUT_FILENO, "\n", 1);
        /* Solo cuando estamos esperando el prompt */
        if (g_interactive)
        {
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
        }
        /* si g_interactive==0 (estamos dentro de un comando), no redibujamos */
    }
    else if (sign == SIGQUIT)
    {
        if (g_interactive)
        {
            rl_on_new_line();
            rl_redisplay();
        }
    }
}

/* Manejador de señales para procesos hijos */
void handle_child_signal(int sign)
{
    if (sign == SIGINT)
        _exit(130);
    else if (sign == SIGQUIT)
    {
        write(STDERR_FILENO, "Quit: 3\n", 8);
        _exit(131);
    }
}

/* Instalación de manejadores para el shell interactivo */
void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_parent_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

/* Instalación de manejadores para los procesos hijos */
void setup_child_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_child_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}