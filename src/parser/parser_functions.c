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

#include "minishell.h"

int	validate_and_split_input(t_msh *shell, char *inputLine, char ***segments)
{
	/* 1. Verifica que las comillas estén balanceadas.
	   check_quotes_balance retorna un valor distinto de 0 si están balanceadas.
	*/
	shell->quote = check_quotes_balance(inputLine, shell);
	if (shell->quote == 0)
	{
		free(inputLine);
		return (1);
	}
	/* 2. Cuenta el número de pipes en la línea.
	   count_pipes retorna -1 si hay error de sintaxis (por ejemplo, pipe mal colocado).
	*/
	shell->pipe = count_pipes(inputLine);
	if (shell->pipe == -1)
	{
		free(inputLine);
		return (1);
	}
	/* 3. Divide la línea en segmentos utilizando el pipe ('|') como delimitador.
	   split_pipes devuelve un arreglo de cadenas (cada una es un segmento).
	*/
	*segments = split_pipes(inputLine);
	if (!(*segments))
	{
		free(inputLine);
		return (1);
	}
	/* Si todo es correcto, retorna 0 indicando que no hubo errores */
	return (0);
}
/*
funciones por añadir a este archivo:

create_command_node

perform_expansion

get_last_command_node

*/