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

char *preprocess_redirections(const char *line)
{
    size_t  i = 0, j = 0;
    char    *out;

    /* Reservamos espacio: posible inserción de espacios extra */
    out = malloc(ft_strlen(line) * 2 + 1);
    if (!out)
        exit_error("Error malloc", 1);

    while (line[i])
    {
        /* Here-doc '<<' */
        if (line[i] == '<' && line[i+1] == '<')
        {
            out[j++] = '<';
            out[j++] = '<';
            i += 2;
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        /* Append '>>' */
        if (line[i] == '>' && line[i+1] == '>')
        {
            out[j++] = '>';
            out[j++] = '>';
            i += 2;
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        /* Redirección de error '2>' o '2>>' */
        if (line[i] == '2' && line[i+1] == '>')
        {
            out[j++] = '2';
            if (line[i+2] == '>')
            {
                out[j++] = '>';
                out[j++] = '>';
                i += 3;
            }
            else
            {
                out[j++] = '>';
                i += 2;
            }
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        /* Redirección simple '<' o '>' */
        if (line[i] == '<' || line[i] == '>')
        {
            out[j++] = line[i++];
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        /* Copia cualquier otro carácter */
        out[j++] = line[i++];
    }
    out[j] = '\0';
    return out;
}

void run_shell_loop(t_msh *shell)
{
    char    *raw_line;
    char    *line;
    t_cmd   *old_cmd;

    while (1)
    {
        g_interactive = 1;
        raw_line = readline(WHITE_T "minishell-> " RESET_COLOR);
        if (!raw_line)
            exit_error("exit", shell->error_value);

        if (is_line_empty(raw_line))
        {
            free(raw_line);
            continue;
        }
        add_history(raw_line);

        /* Pre-procesar espacios en redirecciones */
        line = preprocess_redirections(raw_line);
        free(raw_line);

        old_cmd = shell->cmd;
        if (parse_input_line(shell, &shell->cmd, line))
        {
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

            g_interactive = 1;
        }

        if (old_cmd != shell->cmd)
            free_command_list(old_cmd);
        free(line);
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

    setup_signals();
    run_shell_loop(shell);
    cleanup_shell(shell);
    return (0);
}
