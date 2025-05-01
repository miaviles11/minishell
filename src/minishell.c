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


volatile sig_atomic_t g_interactive = 1;

static void handle_extra_arguments(char **argv)
{
    ft_printf("Error: minishell no acepta argumentos adicionales\n");
    ft_printf("Uso: %s\n", argv[0]);
    exit(1);
}

char *preprocess_redirections(const char *line)
{
    size_t i = 0, j = 0;
    char *out;
    char quote = 0;

    out = malloc(ft_strlen(line) * 2 + 1);
    if (!out)
        exit_error("Error malloc", 1);

    while (line[i])
    {
        if (!quote && (line[i] == '"' || line[i] == '\''))
        {
            quote = line[i++];
            out[j++] = quote;
            continue;
        }
        if (quote && line[i] == quote)
        {
            out[j++] = line[i++];
            quote = 0;
            continue;
        }
        if (quote)
        {
            out[j++] = line[i++];
            continue;
        }
        if (line[i] == '<' && line[i+1] == '<')
        {
            out[j++] = '<'; out[j++] = '<'; i += 2;
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        if (line[i] == '>' && line[i+1] == '>')
        {
            out[j++] = '>'; out[j++] = '>'; i += 2;
            if (line[i] && line[i] != ' ')
                out[j++] = ' ';
            continue;
        }
        if (line[i] == '2' && line[i+1] == '>')
        {
            out[j++] = '2';
            if (line[i+2] == '>') { out[j++] = '>'; out[j++] = '>'; i += 3; }
            else { out[j++] = '>'; i += 2; }
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
        out[j++] = line[i++];
    }
    out[j] = '\0';
    return out;
}

void run_shell_loop(t_msh *shell)
{
    char *raw_line;
    char *line;
    t_cmd *old_cmd;
    int interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
        {
            g_interactive = 1;
            raw_line = readline("minishell-> ");
        }
        else
        {
            g_interactive = 0;
            raw_line = get_next_line(STDIN_FILENO);
        }

        if (!raw_line)
            break;

        {
            size_t len = ft_strlen(raw_line);
            if (len > 0 && raw_line[len - 1] == '\n')
                raw_line[len - 1] = '\0';
        }

        if (is_line_empty(raw_line))
        {
            free(raw_line);
            continue;
        }

        if (interactive)
            add_history(raw_line);

        line = preprocess_redirections(raw_line);
        free(raw_line);

        old_cmd = shell->cmd;
        if (parse_input_line(shell, &shell->cmd, line))
        {
            g_interactive = 0;

            if (shell->cmd && shell->cmd->cmd
                && !ft_strncmp(shell->cmd->cmd, "exit", 5))
            {
                char **args = shell->cmd->arg;
                int code = 0;

                if (args && args[0])
                {
                    if (!is_numeric(args[0]))
                    {
                        ft_printf("exit: %s: numeric argument required\n", args[0]);
                        exit(255);
                    }
                    if (args[1])
                    {
                        ft_printf("exit: too many arguments\n");
                        shell->error_value = 1;
                        free_command_list(shell->cmd);
                        free(line);
                        continue;
                    }
                    code = ft_atoi(args[0]);
                }
                exit(code);
            }

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
    if (interactive)
        write(STDOUT_FILENO, "exit\n", 5);
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

    return (shell->error_value);
}
