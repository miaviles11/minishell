/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 12:48:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/06/08 12:48:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**process_complex_arg(char **args, int index, int i, int *offset)
{
	args = insert_argument_at_index(args,
			ft_substr(args[index], 0, i),
			index + (*offset)++);
	args = insert_argument_at_index(args,
			ft_substr(args[index], i,
				find_next_redirect_operator_index(i, args[index]) - i),
			index + (*offset)++);
	i = find_next_redirect_operator_index(i, args[index]);
	if (args[index][i])
		args = insert_argument_at_index(args,
				ft_substr(args[index], i,
					ft_strlen(args[index]) - i),
				index + (*offset)++);
	args = remove_argument_at_index(args, index);
	return (args);
}

static char	**extract_filename_from_arg(char **args, int index, char opChar,
		int offset)
{
	int	i;

	i = 0;
	if (args[index][i] == '2' && args[index][i + 1] == '>')
		i++;
	while (args[index][i] == opChar)
		i++;
	if (!args[index][i])
		return (args);
	if (i == 0)
		return (keep_argument(args, index, opChar));
	return (process_complex_arg(args, index, i, &offset));
}

static void	handle_redirection_by_type(t_cmd *cmd, int rtype, char *file,
		int heredoc_pipe[2])
{
	if (rtype == 4)
		read_here_doc_to_pipe(file, heredoc_pipe[1]);
	else if (rtype == 3)
	{
		if (redirect_input_from_file(cmd, file, 0))
			return ;
	}
	else
		handle_output_redirection(rtype, cmd, file);
}

static void	remove_two_tokens(t_cmd *cmd, int idx)
{
	cmd->arg = remove_argument_at_index(cmd->arg, idx);
	cmd->arg = remove_argument_at_index(cmd->arg, idx);
}

static void	handle_single_rtype(t_cmd *cmd, int pos, int pipefd[2])
{
	int		rtype;
	char	*file;

	rtype = get_redirect_type(cmd->arg[pos]);
	if (rtype == 4)
		file = ft_strdup(cmd->arg[pos + 1]);
	else
		file = str_noquotes(cmd->arg[pos + 1]);
	handle_redirection_by_type(cmd, rtype, file, pipefd);
	remove_two_tokens(cmd, pos);
}

void	process_single_redirection(t_cmd *cmd, int *i, int heredoc_pipe[2])
{
	int	idx;

	idx = find_first_redirect_index(cmd->arg + *i);
	*i += idx;
	cmd->arg = extract_filename_from_arg(cmd->arg, *i,
			get_operator_for_type(get_redirect_type(cmd->arg[*i])), 1);
	if (!get_redirect_type(cmd->arg[*i]))
		(*i)++;
	if (get_redirect_type(cmd->arg[*i + 1]))
		cmd->arg = extract_filename_from_arg(cmd->arg, *i + 1,
				get_operator_for_type(get_redirect_type(cmd->arg[*i + 1])), 1);
	handle_single_rtype(cmd, *i, heredoc_pipe);
}

void	process_redirections(t_cmd *cmd)
{
	int	heredoc_pipe[2];
	int	has_heredoc;

	has_heredoc = 0;
	setup_heredocs_pre_scan(cmd, heredoc_pipe, &has_heredoc);
	apply_redirections(cmd, heredoc_pipe);
	if (has_heredoc)
	{
		close(heredoc_pipe[1]);
		if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
			exit_error("Error al redirigir STDIN para here-docs", 55);
		close(heredoc_pipe[0]);
	}
}
