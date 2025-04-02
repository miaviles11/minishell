/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_utils_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
/*
** extract_command:
**   Extrae el nombre del comando desde el segmento de entrada, saltándose
**   espacios iniciales y deteniéndose al encontrar un espacio, carácter de control
**   (ASCII 9-13) o un operador de redirección. Si no se encuentra un comando válido,
**   retorna "CD" como valor por defecto.
**
** Parámetros:
**   shell   : Puntero a la estructura global del shell (t_msh). Se usa unicamente para actualizar
**             el contador total de caracteres procesados.
**   segment : Cadena que contiene el segmento de la línea de comando a analizar.
**
** Retorna:
**   Una cadena con el nombre del comando extraído.
*/
static char	*extract_command(t_msh *shell, char *segment)
{
	int		i;
	int		start;
	char	*command;

	i = 0;
	// Salta los espacios y caracteres de control iniciales.
	while (segment[i] == ' ' || (segment[i] >= 9 && segment[i] <= 13))
		i++;
	start = i;
	// Recorre el segmento hasta encontrar un espacio, carácter de control o un operador.
	while (segment[i] && segment[i] != ' ' &&
	       !(segment[i] >= 9 && segment[i] <= 13) &&
	       !is_redirect_operator(segment[i]))
	{
		// Si se encuentra una comilla, salta el bloque entre comillas.
		if (segment[i] == '"' || segment[i] == '\'')
			i = get_next_quote(i + 1, segment, segment[i]);
		i++;
	}
	// Si se extrajo algún contenido válido, se crea la subcadena.
	if (i > start && !is_redirect_operator(segment[i]))
		command = ft_substr(segment, start, i - start);
	else
		command = ft_strdup("CD");
	// Actualiza el contador global de caracteres procesados.
	shell->total_chars += i;
	return (command);
}

int	count_arguments(char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		// Si el carácter actual no es un espacio ni comilla
		// y el siguiente es un delimitador (espacio, fin de cadena o comilla),
		// se considera el final de un argumento.
		if ((s[i] != ' ' && s[i] != '"' && s[i] != '\'') &&
		    (s[i + 1] == ' ' || s[i + 1] == '\0' ||
		     s[i + 1] == '"' || s[i + 1] == '\''))
			count++;
		// Si se encuentra una comilla, se considera un argumento completo
		// y se salta todo el contenido entre las comillas.
		if (s[i] == '"' || s[i] == '\'')
		{
			count++;
			i = get_next_quote(i + 1, s, s[i]);
		}
		i++;
	}
	return (count);
}
