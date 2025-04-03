/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:31:25 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/03 16:32:24 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** has_variable:
**   Recorre la cadena 's' y retorna 1 si detecta la presencia de un
**   signo '$' que indica una variable de entorno (y cuyo siguiente carácter
**   no es un espacio). Ignora el contenido entre comillas simples cuando estamos
**   fuera de comillas dobles, ya que las variables no se expanden en ellas.
**
** Retorna:
**   1 si se encuentra una variable, o 0 si no.
*/
int	has_variable(const char *s)
{
	int	i = 0;
	int	dq_flag = 1;  // 1: fuera de comillas dobles; -1: dentro de comillas dobles.

	while (s[i])
	{
		// Si encontramos una comilla simple y estamos fuera de comillas dobles, saltamos su contenido.
		if (s[i] == '\'' && dq_flag == 1)
			i = get_next_quote(i + 1, (char *)s, '\'');
		// Alterna el estado al encontrar una comilla doble.
		if (s[i] == '"')
			dq_flag *= -1;
		// Si se detecta un '$' seguido de un carácter que no sea espacio, se asume que hay una variable.
		if (s[i] == '$' && s[i + 1] && s[i + 1] != ' ')
			return (1);
		i++;
	}
	return (0);
}
