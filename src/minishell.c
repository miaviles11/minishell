/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/20 16:59:54 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


volatile sig_atomic_t g_interactive = 1;  // 1 = esperando prompt; 0 = ejecutando comando

static void handle_extra_arguments(char **argv)
{
    ft_printf("Error: minishell no acepta argumentos adicionales\n");
    ft_printf("Uso: %s\n", argv[0]);
    exit(1);
}

void run_shell_loop(t_msh *shell)
{
    char *input_line;
    t_cmd *old_cmd;

    while (1)
    {
        /* Estamos esperando el prompt */
        g_interactive = 1;
        input_line = readline(WHITE_T "minishell-> " RESET_COLOR);
        if (!input_line)
            exit_error("exit", shell->error_value);

        if (is_line_empty(input_line))
        {
            free(input_line);
            continue;
        }
        add_history(input_line);

        old_cmd = shell->cmd;
        if (parse_input_line(shell, &shell->cmd, input_line))
        {
            /* A partir de aquí, ejecutamos un comando, ya no estamos en prompt */
            g_interactive = 0;

            if (!shell->pipe
                && shell->cmd
                && shell->cmd->cmd
                && is_builtin(shell->cmd->cmd))
            {
                execute_builtin(shell, shell->cmd);
            }
            else if (shell->cmd)
            {
                execute_commands(shell);
            }

            /* Fin de ejecución, volver a modo prompt */
            g_interactive = 1;
        }

        if (old_cmd != shell->cmd)
            free_command_list(old_cmd);
        free(input_line);
    }
}

int main(int argc, char **argv, char **envp)
{
    t_msh *shell;

    if (argc != 1)
        handle_extra_arguments(argv);

    shell = initialize_shell(envp);
    if (!shell)
        return (1);
    shell->error_value = 0;

    /* Instalar handlers de señales del padre */
    setup_signals();

    run_shell_loop(shell);
    cleanup_shell(shell);
    return (0);
}