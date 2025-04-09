/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arguments_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** split_arguments:
**   Divide la cadena 's' en argumentos, separándolos por espacios (y caracteres de control)
**   y respetando las comillas (utilizando get_next_quote para saltar el contenido entre comillas).
**
**   Cuando se detecta el final de un argumento (el siguiente carácter es un espacio, fin de cadena,
**   o una comilla), se extrae la subcadena con ft_substr y se almacena en el arreglo 'arg'.
**
** Retorna:
**   El número total de caracteres procesados (útil para actualizar contadores globales).
*/
static int	split_arguments(char *s, char **arg)
{
	int	pos;
	int	start;
	int	i;

	start = 0;
	pos = 0;
	i = 0;
	while (s[i])
	{
		// Actualiza el inicio del siguiente argumento al encontrar delimitadores.
		if ((s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)) &&
		    s[i + 1] && s[i + 1] != ' ')
			start = i + 1;
		// Si se encuentra una comilla, salta el contenido entre comillas.
		if (s[i] == '"' || s[i] == '\'')
			i = get_next_quote(i + 1, s, s[i]);
		// Si se detecta el final de un argumento, extrae el argumento.
		if (s[i] != ' ' && !(s[i] >= 9 && s[i] <= 13) &&
		    (s[i + 1] == ' ' || s[i + 1] == '\0'))
		{
			arg[pos] = ft_substr(s, start, i - start + 1);
			if (!arg[pos])
				exit_error("Error malloc", 14);
			pos++;
		}
		i++;
	}
	arg[pos] = NULL;
	return (i);
}

char	**extract_arguments(t_msh *shell, char *segment, t_cmd *command)
{
	char	**args;
	char	*processedSegment;

	processedSegment = segment;
	// Si se detectan argumentos nulos en el segmento, se corrige la cadena.
	if (check_null_args(processedSegment) == -1)
		processedSegment = change_null_args(ft_strtrim(processedSegment, " "), command);
	// Reserva memoria para el arreglo de argumentos (command->num_arg + 1 para el terminador NULL)
	args = malloc(sizeof(char *) * (command->num_arg + 1));
	if (!args)
		exit_error("Error malloc", 13);
	// Divide la cadena en argumentos y actualiza el contador global de caracteres procesados.
	shell->total_chars += split_arguments(segment, args);
	// Si se detectaron argumentos nulos, libera la versión temporal corregida.
	if (check_null_args(segment) == -1)
		free(processedSegment);
	return (args);
}


int	check_null_args(char *s)
{
	int	i = 0;
	int	dq_flag = 1;  // Bandera para comillas dobles.
	int	sq_flag = 1;  // Bandera para comillas simples.

	while (s[i])
	{
		if (s[i] == '"')
			dq_flag *= -1;
		if (s[i] == '\'')
			sq_flag *= -1;
		// Patrón: espacio, "", espacio.
		if (s[i] == ' ' && s[i + 1] == '"' && s[i + 2] == '"' && s[i + 3] == ' ')
			return (-1);
		// Patrón: espacio, '', espacio.
		else if (s[i] == ' ' && s[i + 1] == '\'' && s[i + 2] == '\'' && s[i + 3] == ' ')
			return (-1);
		// Patrón: dos comillas consecutivas sin el balance esperado.
		else if ((s[i] == '"' && s[i + 1] == '"' && sq_flag != -1) ||
		         (s[i] == '\'' && s[i + 1] == '\'' && dq_flag != -1))
			return (-1);
		i++;
	}
	return (0);
}

/*
** change_null_args:
**   Corrige o elimina patrones de "argumentos nulos" en la cadena 's'.
**   Si se detecta un espacio seguido de comillas consecutivas (o viceversa) que
**   indiquen un argumento vacío, se invoca quit_null_space para ajustar la cadena.
**   Además, si se detectan dos comillas consecutivas ("" o ''), se eliminan mediante quit_null,
**   y se decrementa el contador de argumentos en el comando.
**   La función se llama recursivamente hasta que se hayan corregido todos los patrones.
**
** Retorna la cadena 's' modificada.
*/
char	*change_null_args(char *s, t_cmd *cmd)
{
	int	i = 0;

	while (s[i])
	{
		// Si se encuentra una comilla que no se repite inmediatamente, saltar el bloque entre comillas.
		if ((s[i] == '"' && s[i + 1] && s[i + 1] != '"') ||
		    (s[i] == '\'' && s[i + 1] && s[i + 1] != '\''))
			i = get_next_quote(i + 1, s, s[i]);
		// Patrón: espacio seguido de comillas consecutivas y luego espacio o fin de cadena.
		if (s[i] == ' ' &&
		    (s[i + 1] == '"' || s[i + 1] == '\'') &&
		    (s[i + 2] == '"' || s[i + 2] == '\'') &&
		    (s[i + 3] == ' ' || s[i + 3] == '\0'))
		{
			i++;
			s = quit_null_space(s, i);
			s = change_null_args(s, cmd);
		}
		// Si se detectan dos comillas consecutivas, se eliminan y se decrementa el contador de argumentos.
		else if ((s[i] == '"' && s[i + 1] == '"') ||
		         (s[i] == '\'' && s[i + 1] == '\''))
		{
			s = quit_null(s, i);
			cmd->num_arg--;
			return (change_null_args(s, cmd));
		}
		i++;
	}
	return (s);
}

