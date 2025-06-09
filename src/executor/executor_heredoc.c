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
char *expand_env(char *in)
{
	int  i;
	char *out;

	out = ft_calloc(1, 1);
	i = 0;
	while (in[i])
	{
		if (in[i] == '$' && ft_isalnum(in[i + 1]))
		{
			int start = ++i;
			while (ft_isalnum(in[i]) || in[i] == '_')
				i++;
			char *var  = ft_substr(in, start, i - start);
			char *val  = getenv(var);
			out = ft_strjoin_free(out, val ? val : "");
			free(var);
		}
		else
		{
			out = ft_chrjoin_free(out, in[i++]);
		}
	}
	return (out);
}
