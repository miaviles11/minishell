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

/**
 * Inserta espacios alrededor de los operadores de redirección
 * SOLO cuando no esté dentro de comillas simples o dobles.
 */
char *preprocess_redirections(const char *line)
{
    size_t  i = 0, j = 0;
    char    *out;
    char    quote = 0;

    /* Reservamos hasta el doble de espacio + \0 */
    out = malloc(ft_strlen(line) * 2 + 1);
    if (!out)
        exit_error("Error malloc", 1);

    while (line[i])
    {
        /* Abrir comillas */
        if (!quote && (line[i] == '"' || line[i] == '\''))
        {
            quote = line[i++];
            out[j++] = quote;
            continue;
        }
        /* Cerrar comillas */
        if (quote && line[i] == quote)
        {
            out[j++] = line[i++];
            quote = 0;
            continue;
        }
        /* Si estamos dentro de comillas, copiamos literalmente */
        if (quote)
        {
            out[j++] = line[i++];
            continue;
        }

        /* Aquí NO hay comillas: tratamos redirecciones */
        if (line[i] == '<' && line[i+1] == '<')
        {
            out[j++] = '<';
            out[j++] = '<';
            i += 2;
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        if (line[i] == '>' && line[i+1] == '>')
        {
            out[j++] = '>';
            out[j++] = '>';
            i += 2;
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
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
        if (line[i] == '<' || line[i] == '>')
        {
            out[j++] = line[i++];
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }

        /* Copia normal */
        out[j++] = line[i++];
    }

    out[j] = '\0';
    return out;
}

/**
 * Bucle principal de lectura-ejecución.
 * Ahora aplica preprocess_redirections() justo después de readline().
 */
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

        /* --- Nuevo paso: preprocesar redirecciones --- */
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
