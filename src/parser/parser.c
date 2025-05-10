/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:28:32 by miaviles          #+#    #+#             */
/*   Updated: 2025/05/10 11:28:32 by miaviles         ###   ########.fr       */
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

int	parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine)
{
	int		i;
	t_cmd	*newcommand;
	char	**splitsegments;

	shell->total_chars = 0;
	if (validate_and_split_input(shell, inputLine, &splitsegments))
		return (0);
	*commandList = create_command_node(shell, splitsegments[0]);
	i = 1;
	while (i <= shell->pipe)
	{
		shell->total_chars = 0;
		newcommand = create_command_node(shell, splitsegments[i]);
		get_last_command_node(commandList)->next = newcommand;
		i++;
	}
	i = 0;
	while (splitsegments[i])
		free(splitsegments[i++]);
	free(splitsegments);
	return (1);
}
