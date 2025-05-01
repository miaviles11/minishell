/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_and_split_input(t_msh *shell, char *inputLine, char ***segments)
{
	shell->quote = check_quotes_balance(inputLine, shell);
	if (shell->quote == 0)
	{
		return (1);
	}
	shell->pipe = count_pipes(inputLine, shell);
    if (shell->pipe == -1)
	{
        return (1);
	}
	*segments = split_pipes(inputLine, shell);
    if (!(*segments))
	{
        return (1);
	}
	return (0);
}

t_cmd	*create_command_node(t_msh *shell, char *segment)
{
	int		cmd_idx;
	t_cmd	*node;

	node = ft_calloc(1, sizeof(t_cmd));
	if (!node)
		exit_error("Error malloc", 9);

	node->num_arg = count_arguments_parser(segment);
	if (node->num_arg > 0)
		node->arg = extract_arguments(shell, segment, node);
	else
		node->arg = NULL;

	cmd_idx = 0;
	while (node->arg
		&& node->arg[cmd_idx]
		&& get_redirect_type(node->arg[cmd_idx]) > 0)
		cmd_idx += 2;

	if (node->arg && node->arg[cmd_idx])
	{
		node->cmd = ft_strdup(node->arg[cmd_idx]);
		node->arg = remove_argument_at_index(node->arg, cmd_idx);
		node->num_arg--;
	}
	else
	{
		node->cmd = ft_strdup("");
	}

	node->env  = shell->env;
	node->next = NULL;
	return (node);
}

void	perform_expansion(t_msh *msh, t_cmd **command)
{
	int		i;
	char	*var_reminder;
	char	*tmp;

	var_reminder = NULL;

	if (has_variable((*command)->cmd))
		(*command)->cmd = substitute_variables(
				msh, *command, (*command)->cmd, &var_reminder);
	if (needs_home_expansion((*command)->cmd))
		(*command)->cmd = expand_home_directory((*command)->cmd);
	tmp = str_noquotes((*command)->cmd);
	free((*command)->cmd);
	(*command)->cmd = tmp;

	if (!(*command)->arg)
		return ;

	i = 0;
	while ((*command)->arg[i])
	{
		if (has_variable((*command)->arg[i]))
			(*command)->arg[i] = substitute_variables(
					msh, *command, (*command)->arg[i], &var_reminder);
		if (needs_home_expansion((*command)->arg[i]))
			(*command)->arg[i] = expand_home_directory(
					(*command)->arg[i]);
		tmp = str_noquotes((*command)->arg[i]);
		free((*command)->arg[i]);
		(*command)->arg[i] = tmp;
		i++;
	}
}

t_cmd	*get_last_command_node(t_cmd **cmd)
{
	t_cmd	*current;

	current = *cmd;
	while (current && current->next)
		current = current->next;
	return (current);
}
