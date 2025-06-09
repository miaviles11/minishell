/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:17:36 by miaviles          #+#    #+#             */
/*   Updated: 2025/05/05 17:19:40 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	read_here_doc_lines(char *delimiter)
{
	char	*line;

	while (1)
	{
		write(STDERR_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
		{
			free(line);
			break ;
		}
		free(line);
	}
}

void	consume_here_docs(char **args)
{
	int		i;
	char	*delimiter;

	i = 0;
	while (args && args[i])
	{
		if (get_redirect_type(args[i]) == 4)
		{
			delimiter = str_noquotes(args[i + 1]);
			read_here_doc_lines(delimiter);
			free(delimiter);
			i += 2;
		}
		else
			i++;
	}
}
static char	*join_and_free(char *s1, const char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	free(s1);
	return (tmp);
}

static char	*charjoin_and_free(char *s1, char c)
{
	char	tmp[2];
	char	*tmpstr;

	tmp[0] = c;
	tmp[1] = '\0';
	tmpstr = ft_strjoin(s1, tmp);
	free(s1);
	return (tmpstr);
}

static char	*expand_variable(const char *in, size_t *i)
{
	size_t	start;
	char	*var;
	char	*val;
	char	*ret;

	start = ++(*i);
	while (in[*i] && (ft_isalnum(in[*i]) || in[*i] == '_'))
		(*i)++;
	var = ft_substr(in, start, *i - start);
	val = getenv(var);
	if (val != NULL)
		ret = ft_strdup(val);
	else
		ret = ft_strdup("");
	free(var);
	return (ret);
}

char	*expand_env(const char *in)
{
	size_t	i;
	char	*out;
	char	*tmp;

	out = ft_calloc(1, 1);
	if (!out)
		exit_error("Error malloc", 1);
	i = 0;
	while (in[i])
	{
		if (in[i] == '$' && (ft_isalnum(in[i + 1]) || in[i + 1] == '_'))
		{
			tmp = expand_variable(in, &i);
			out = join_and_free(out, tmp);
			free(tmp);
		}
		else
			out = charjoin_and_free(out, in[i++]);
	}
	return (out);
}
