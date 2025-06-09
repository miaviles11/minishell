/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 12:47:59 by carlsanc          #+#    #+#             */
/*   Updated: 2025/06/08 12:47:59 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	heredoc_child(const char *delimiter, int write_fd)
{
	int	tty_fd;

	setup_child_signals();
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
		exit_error("Error al abrir /dev/tty para here-doc", 47);
	read_here_doc_loop(delimiter, write_fd, tty_fd);
	close(tty_fd);
	close(write_fd);
	_exit(0);
}

static int	is_quoted_delim(const char *delim)
{
	if (delim[0] == '\'' || delim[0] == '\"')
		return (1);
	return (0);
}

static char	*clean_delim(const char *delim, int quoted)
{
	if (quoted)
		return (str_noquotes((char *)delim));
	return ((char *)delim);
}

static int	handle_here_doc_line(char *clean, int quoted,
								int write_fd, int tty_fd)
{
	char	*line;
	char	*tmp;

	if (write(STDERR_FILENO, "> ", 2) == -1)
		exit_error("Error de escritura en prompt", 48);
	line = get_next_line(tty_fd);
	if (!line)
		exit_error("EOF inesperado en here-doc", 53);
	if (!ft_strncmp(line, clean, ft_strlen(clean))
		&& line[ft_strlen(clean)] == '\n')
	{
		free(line);
		return (0);
	}
	if (!quoted)
	{
		tmp = expand_env(line);
		if (write(write_fd, tmp, ft_strlen(tmp)) == -1)
			exit_error("Error al escribir en here-doc", 54);
		free(tmp);
	}
	else if (write(write_fd, line, ft_strlen(line)) == -1)
		exit_error("Error al escribir en here-doc", 54);
	free(line);
	return (1);
}

void	read_here_doc_loop(const char *delimiter, int write_fd, int tty_fd)
{
	int		quoted;
	char	*clean;

	quoted = is_quoted_delim(delimiter);
	clean = clean_delim(delimiter, quoted);
	while (handle_here_doc_line(clean, quoted, write_fd, tty_fd))
		;
	if (quoted)
		free(clean);
}

void	read_here_doc_to_pipe(const char *delimiter, int write_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		exit_error("fork heredoc", 60);
	if (pid == 0)
		heredoc_child(delimiter, write_fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_parent_signal);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(write_fd);
		g_interactive = 1;
	}
	free((char *)delimiter);
}

void	setup_heredoc_pipe(int *heredoc_pipe, int *has_heredoc)
{
	if (!(*has_heredoc))
	{
		if (pipe(heredoc_pipe) == -1)
			exit_error("Error al crear pipe para here-docs", 47);
		*has_heredoc = 1;
	}
}
