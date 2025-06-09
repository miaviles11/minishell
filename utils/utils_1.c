/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_line_empty(const char *s)
{
	int	i;
	int	whitespace_count;

	i = 0;
	whitespace_count = 0;
	if (!s || s[0] == '\0')
		return (1);
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'
			|| s[i] == '\v' || s[i] == '\f' || s[i] == '\r')
			whitespace_count++;
		i++;
	}
	if (whitespace_count == i)
		return (1);
	return (0);
}

char	*str_noquotes(char *str)
{
	int		i;
	int		j;
	char	*temp;
	int		in_dquote;
	int		in_squote;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	temp = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!temp)
		exit_error("Error malloc", 11);
	while (str[i])
	{
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else
			temp[j++] = str[i];
		i++;
	}
	return (temp);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	init_here_doc(int p[2], int *tty_fd)
{
	if (pipe(p) == -1)
		exit_error("Error al crear pipe para here-document", 47);
	*tty_fd = open("/dev/tty", O_RDONLY);
	if (*tty_fd < 0)
		exit_error("Error al abrir /dev/tty para here-document", 52);
}

static int	is_quoted_hd(const char *d)
{
	if (d[0] == '\'' || d[0] == '\"')
		return (1);
	return (0);
}

/* <= 18 líneas */
static void	write_hd_line(char *line, int quoted, int wfd)
{
	char	*tmp;

	if (!quoted)
	{
		tmp = expand_env(line);
		if (write(wfd, tmp, ft_strlen(tmp)) == -1)
			exit_error("write heredoc", 54);
		free(tmp);
	}
	else if (write(wfd, line, ft_strlen(line)) == -1)
		exit_error("write heredoc", 54);
}

/* --------------- función principal ≤ 25 líneas ---------------------- */
void	read_here_doc(char *delimiter, int write_fd)
{
	int		quoted = is_quoted_hd(delimiter);
	char	*clean = (char *)delimiter;
	char	*line;

	if (quoted)
		clean = str_noquotes(delimiter);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			exit_error("EOF heredoc", 53);
		if (!ft_strncmp(line, clean, ft_strlen(clean)) &&
			line[ft_strlen(clean)] == '\n')
		{
			free(line);
			break ;
		}
		write_hd_line(line, quoted, write_fd);
		free(line);
	}
	if (quoted)
		free(clean);
}
