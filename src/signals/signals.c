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

extern volatile sig_atomic_t g_interactive;

/* Handler para Ctrl‑C y Ctrl‑\\ en el padre */
void handle_parent_signal(int sign)
{
    if (sign == SIGINT)  /* Ctrl‑C */
    {
        write(STDOUT_FILENO, "\n", 1);
        if (g_interactive)
        {
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
        }
    }
    else if (sign == SIGQUIT)  /* Ctrl‑\\ */
    {
        if (!g_interactive)  /* solo si un comando estaba corriendo */
        {
            /* El TTY ya imprimió "^\" y salto de línea */
            write(STDERR_FILENO, "Quit: 3\n", 8);
        }
        else  /* prompt vacío: redibujar */
        {
            rl_on_new_line();
            rl_replace_line("", 0);
            rl_redisplay();
        }
    }
}

/* Handler para SIGINT y SIGQUIT en los hijos */
void handle_child_signal(int sign)
{
    if (sign == SIGINT)
        _exit(130);
    else if (sign == SIGQUIT)
        _exit(131);
}

void setup_signals(void)
{
    struct sigaction sa;

    /* Padre: capturar Ctrl‑C y Ctrl‑\\ */
    sa.sa_handler = handle_parent_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    /* Ignorar SIGTERM en el padre */
    sa.sa_handler = SIG_IGN;
    sigaction(SIGTERM, &sa, NULL);
}

void setup_child_signals(void)
{
    struct sigaction sa;

    /* Hijos: manejar SIGINT y SIGQUIT */
    sa.sa_handler = handle_child_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}