/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
** count_valid_pipes:
**   Recorre la cadena 'inputLine' ignorando el contenido entre comillas,
**   valida la sintaxis de los pipes y cuenta los pipes válidos.
**   Retorna el número de pipes encontrados o -1 en caso de error.
*/
static int	count_valid_pipes(const char *inputLine)
{
	int	i = 0;
	int	pipeCount = 0;

	while (inputLine[i])
	{
		if (inputLine[i] == '"' || inputLine[i] == '\'')
		{
			i = skip_quoted_segment(inputLine, i, inputLine[i]);
			continue;
		}
		if (inputLine[i] == '|')
		{
			if (inputLine[i + 1] == '|' || inputLine[i + 1] == '\0')
			{
				put_error("bash", NULL, "syntax error near unexpected token `|'");
				g_error = 258;
				return (-1);
			}
			pipeCount++;
		}
		i++;
	}
	return (pipeCount);
}

/*
** count_pipes:
**   Verifica que la línea no comience con un pipe y luego cuenta los pipes válidos.
**   Retorna el número de pipes o -1 si ocurre un error de sintaxis.
*/
int	count_pipes(const char *inputLine)
{
	if (inputLine[0] == '|')
	{
		put_error("bash", NULL, "syntax error near unexpected token `|'");
		g_error = 258;
		return (-1);
	}
	return (count_valid_pipes(inputLine));
}

/*
** skip_quoted_segment:
**   Avanza el índice 'i' en la cadena 's' para saltar el contenido delimitado
**   por la comilla 'quote'. Se asume que s[i] es la comilla de apertura.
**   Retorna el nuevo valor de i, que apunta justo después de la comilla de cierre.
*/
static int	skip_quoted_segment(const char *s, int i, char quote)
{
	i++;  // Salta la comilla de apertura
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;  // Salta la comilla de cierre
	return (i);
}