/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/20 16:46:17 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**extract_filename_from_arg(char **args, int index, char opChar, int offset)
{
	int	i;

	i = 0;
	// Si el argumento empieza con "2>", salta el '2'
	if (args[index][i] == '2' && args[index][i + 1] == '>')
		i++;
	// Salta todos los caracteres del operador (por ejemplo, '>' o '<')
	while (args[index][i] == opChar)
		i++;
	// Si después del operador no queda nada, no se hace modificación.
	if (!args[index][i])
		return (args);
	// Si no se ha saltado ningún operador, utiliza keep_argument para ajustar el argumento.
	if (i == 0)
		return (keep_argument(args, index, opChar));
	// Inserta la parte anterior al operador en el arreglo.
	args = insert_argument_at_index(args, ft_substr(args[index], 0, i), index + offset++);
	// Inserta la parte correspondiente al operador.
	args = insert_argument_at_index(args, ft_substr(args[index], i,
				find_next_redirect_operator_index(i, args[index]) - i),
				index + offset++);
	// Actualiza 'i' para avanzar hasta el final de la parte del operador.
	i = find_next_redirect_operator_index(i, args[index]);
	// Si queda parte adicional (el sufijo), la inserta en el arreglo.
	if (args[index][i])
		args = insert_argument_at_index(args, ft_substr(args[index], i,
							ft_strlen(args[index]) - i),
							index + offset++);
	// Elimina el elemento original, ya que fue dividido en varias partes.
	args = remove_argument_at_index(args, index);
	return (args);
}

static void read_here_doc_to_pipe(const char *delimiter, int write_fd)
{
    char    *line;
    int     tty_fd;

    tty_fd = open("/dev/tty", O_RDONLY);
    if (tty_fd < 0)
        exit_error("Error al abrir /dev/tty para here-doc", 47);

    while (1)
    {
        if (write(STDERR_FILENO, "> ", 2) == -1)
            exit_error("Error de escritura en prompt", 48);
        line = get_next_line(tty_fd);
        if (!line)
            exit_error("EOF inesperado en here-doc", 53);
        if (!ft_strncmp(line, delimiter, ft_strlen(delimiter))
            && line[ft_strlen(delimiter)] == '\n')
        {
            free(line);
            break;
        }
        if (write(write_fd, line, ft_strlen(line)) == -1)
            exit_error("Error al escribir en pipe de here-doc", 54);
        free(line);
    }
    free((void *)delimiter);
    close(tty_fd);
}

void process_redirections(t_cmd *cmd)
{
    int     i = 0;
    int     idx;
    int     rtype;
    char    *file;
    int     heredoc_pipe[2];
    int     has_heredoc = 0;

    // First pass: detect here-docs and open pipe if needed
    while ((idx = find_first_redirect_index(cmd->arg + i)) != -1)
    {
        if (get_redirect_type(cmd->arg[i + idx]) == 4)
        {
            if (!has_heredoc)
            {
                if (pipe(heredoc_pipe) == -1)
                    exit_error("Error al crear pipe para here-docs", 47);
                has_heredoc = 1;
            }
        }
        i += idx + 2;
    }

    // Second pass: process each operator
    i = 0;
    while ((idx = find_first_redirect_index(cmd->arg + i)) != -1)
    {
        i += idx;
        cmd->arg = extract_filename_from_arg(cmd->arg, i,
                    get_operator_for_type(get_redirect_type(cmd->arg[i])), 1);
        if (!get_redirect_type(cmd->arg[i]))
            i++;
        if (get_redirect_type(cmd->arg[i + 1]))
            cmd->arg = extract_filename_from_arg(cmd->arg, i + 1,
                        get_operator_for_type(get_redirect_type(cmd->arg[i + 1])), 1);
        file = str_noquotes(cmd->arg[i + 1]);
        rtype = get_redirect_type(cmd->arg[i]);

        if (rtype == 4)
        {
            // Here-doc: append to shared pipe
            read_here_doc_to_pipe(file, heredoc_pipe[1]);
        }
        else if (rtype == 3)
        {
            // Simple input redirection
            if (redirect_input_from_file(cmd, file, i))
                return; // on error, abort
        }
        else
        {
            // Output (>, >>, 2>, 2>>)
            handle_output_redirection(rtype, cmd, file);
        }
        // Remove processed args (operator and operand)
        cmd->arg = remove_argument_at_index(cmd->arg, i);
        cmd->arg = remove_argument_at_index(cmd->arg, i);
    }

    // After all here-docs, if any, redirect STDIN to the concatenated pipe
    if (has_heredoc)
    {
        close(heredoc_pipe[1]);
        if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
            exit_error("Error al redirigir STDIN para here-docs", 55);
        close(heredoc_pipe[0]);
    }
}

