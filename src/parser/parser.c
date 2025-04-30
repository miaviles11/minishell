/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine)
{
    int     i;
    t_cmd   *newCommand;
    char    **splitSegments;

    shell->total_chars = 0;
    if (validate_and_split_input(shell, inputLine, &splitSegments))
        return (0);

    *commandList = create_command_node(shell, splitSegments[0]);
    i = 1;
    while (i <= shell->pipe)
    {
        shell->total_chars = 0;
        newCommand = create_command_node(shell, splitSegments[i]);
        get_last_command_node(commandList)->next = newCommand;
        i++;
    }

    i = 0;
    while (splitSegments[i])
        free(splitSegments[i++]);
    free(splitSegments);
    return (1);
}

