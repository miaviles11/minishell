/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:59:06 by miaviles          #+#    #+#             */
/*   Updated: 2025/05/20 19:20:05 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*preprocess_redirections(const char *line)
{
	size_t	i;
	size_t	j;
	char	*out;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	out = malloc(ft_strlen(line) * 2 + 1);
	if (!out)
		exit_error("Error malloc", 1);
	while (line[i])
	{
		if (handle_quotes(&i, &j, out, line, &quote))
			continue ;
		if (handle_double_redirections(&i, &j, out, line))
			continue ;
		if (handle_stderr_redirection(&i, &j, out, line))
			continue ;
		if (handle_single_redirection(&i, &j, out, line))
			continue ;
		out[j++] = line[i++];
	}
	out[j] = '\0';
	return (out);
}

int	handle_quotes(size_t *i, size_t *j, char *out, const char *line,
	char *quote)
{
	if (!*quote && (line[*i] == '"' || line[*i] == '\''))
	{
		*quote = line[*i];
		out[(*j)++] = line[(*i)++];
		return (1);
	}
	if (*quote && line[*i] == *quote)
	{
		out[(*j)++] = line[(*i)++];
		*quote = 0;
		return (1);
	}
	if (*quote)
	{
		out[(*j)++] = line[(*i)++];
		return (1);
	}
	return (0);
}

int	handle_double_redirections(size_t *i, size_t *j, char *out,
	const char *line)
{
	if (line[*i] == '<' && line[*i+1] == '<')
	{
		out[(*j)++] = '<'; out[(*j)++] = '<'; (*i) += 2;
		if (line[*i] && line[*i] != ' ')
			out[(*j)++] = ' ';
		return (1);
	}
	if (line[*i] == '>' && line[*i+1] == '>')
	{
		out[(*j)++] = '>'; out[(*j)++] = '>'; (*i) += 2;
		if (line[*i] && line[*i] != ' ')
			out[(*j)++] = ' ';
		return (1);
	}
	return (0);
}

int	handle_stderr_redirection(size_t *i, size_t *j, char *out,
	const char *line)
{
	if (line[*i] == '2' && line[*i+1] == '>')
	{
		out[(*j)++] = '2';
		if (line[*i+2] == '>')
		{
			out[(*j)++] = '>';
			out[(*j)++] = '>';
			(*i) += 3;
		}
		else
		{
			out[(*j)++] = '>';
			(*i) += 2;
		}
		if (line[*i] && line[*i] != ' ')
			out[(*j)++] = ' ';
		return (1);
	}
	return (0);
}

int	handle_single_redirection(size_t *i, size_t *j, char *out, const char *line)
{
	if (line[*i] == '<' || line[*i] == '>')
	{
		out[(*j)++] = line[(*i)++];
		if (line[*i] && line[*i] != ' ')
			out[(*j)++] = ' ';
		return (1);
	}
	return (0);
}
