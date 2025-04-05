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

int	parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine)
{
	int		i;
	t_cmd	*currentCommand;
	t_cmd	*newCommand;
	char	**splitSegments;

	i = 0;
	/* Reinicia el contador de caracteres procesados */
	shell->total_chars = 0;
	/* Valida la sintaxis (balance de comillas) y divide la línea en segmentos según el pipe */
	if (validate_and_split_input(shell, inputLine, &splitSegments))
		return (0);
	/* Crea el primer nodo de comando a partir del primer segmento */
	*commandList = create_command_node(shell, splitSegments[0]);
	/* Realiza la expansión (por ejemplo, variables y eliminación de comillas) sobre el comando */
	perform_expansion(commandList);
	currentCommand = *commandList;
	i = 1;
	/* Recorre cada segmento adicional para formar el pipeline */
	while (i <= shell->pipe)
	{
		shell->total_chars = 0;
		newCommand = create_command_node(shell, splitSegments[i]);
		perform_expansion(&newCommand);
		get_last_command_node(commandList)->next = newCommand;
		currentCommand = newCommand;
		i++;
	}
	/* Libera la memoria reservada para los segmentos y la línea original */
	i = 0;
	while (splitSegments[i])
	{
		free(splitSegments[i]);
		i++;
	}
	free(splitSegments);
	free(inputLine);
	return (1);
}

