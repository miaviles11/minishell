/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:20:24 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/05 10:20:24 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
/*
** check_variable_and_digit:
**   Recorre la cadena 's' hasta encontrar el primer signo '$'. 
**   Si lo encuentra y el carácter inmediatamente siguiente es un dígito 
**   (según ft_isdigit_special), retorna 0 (indicando que se encontró un patrón '$' seguido de dígito).
**   En caso contrario, retorna 1.
*/
int	check_variable_and_digit(const char *s)
{
	int	i = 0;

	while (s[i] && s[i] != '$')
		i++;
	if (s[i] == '$' && s[i + 1] && ft_isdigit_special(s[i + 1]) == 1)
		return (0);
	return (1);
}
/*
** quit_variable_and_digit:
**   Procesa la cadena 's' eliminando todas las ocurrencias del patrón:
**   un signo '$' seguido de un dígito (según ft_isdigit_special).
**
**   Por ejemplo, "$1a $2b" se transforma en "a b".
**
** Parámetros:
**   s: Cadena de entrada que puede contener variables a eliminar.
**   dummy1, dummy2: Parámetros no utilizados (para mantener la firma original).
**
** Retorna:
**   Una nueva cadena con los patrones eliminados. Se libera la cadena original.
*/
char	*quit_variable_and_digit(char *s, int dummy1, int dummy2)
{
	int		i;
	int		j;
	char	*result;
	int		len;

	len = ft_strlen(s);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		exit_error("Error malloc", 15);
	i = 0;
	j = 0;
	while (s[i])
	{
		// Si se detecta un '$' seguido de un dígito, lo salta.
		if (s[i] == '$' && s[i + 1] && ft_isdigit_special(s[i + 1]) == 1)
		{
			i += 2;
			continue;
		}
		result[j++] = s[i++];
	}
	result[j] = '\0';
	free(s);
	return (result);
}
/*
** is_digit_special:
**   Retorna 1 si el carácter 'c' es un dígito (entre '0' y '9') o si es uno de los caracteres especiales:
**   '*', '@' o '\' (ASCII 92). 
**   De lo contrario, retorna 0.
*/
int	is_digit_special(int c)
{
	if (c == '*' || c == '@' || c == '\\')
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
/*
** substitute_variable_value:
**   Procesa la cadena 'line' para sustituir la primera variable de entorno
**   (indicada por '$') por su valor correspondiente.
**
**   - Si se encuentra el patrón "$?", se llama a replace_special_value.
**   - Extrae el nombre de la variable mediante extract_variable_name.
**   - Recorre la cadena a partir del primer '$' (usando find_next_dollar) hasta encontrar
**     un espacio o un carácter especial.
**   - Si se detecta un carácter especial (o un segundo '$' en cierta posición),
**     se asigna parte de la cadena a varReminder mediante split_variable_reminder.
**   - Finalmente, se utiliza compare_variable_name para construir la cadena con la variable
**     sustituida.
**
** Parámetros:
**   cmd         : Nodo de comando (t_cmd) que contiene el entorno y flags.
**   line        : Cadena de entrada que contiene la variable a expandir.
**   varReminder : Cadena auxiliar para retener parte especial, o NULL.
**
** Retorna:
**   La cadena 'line' con la variable sustituida por su valor.
*/
char	*substitute_variable_value(t_cmd *cmd, char *line, char **varReminder)
{
	char	*varName;
	int		i;

	// Si se encuentra el patrón "$?", reemplaza por su valor especial.
	if (ft_strnstr(line, "$?", ft_strlen(line)) != 0)
		return (replace_special_value(line));

	// Extrae el nombre de la variable (por ejemplo, "USER" de "$USER").
	varName = extract_variable_name(line);

	// Busca la posición del primer '$' en la cadena.
	i = find_next_dollar(line, -1);

	// Recorre la cadena a partir de la posición encontrada, hasta un espacio.
	while (line[++i] && line[i] != ' ')
	{
		// Si se encuentra un carácter especial o un segundo '$' en la posición indicada,
		// guarda la parte restante en varReminder.
		if ((special_char_check(line[i]) == -1 && line[i] != '$')
			|| (line[i] == '$' && i == find_next_dollar(line, find_next_dollar(line, -1) + 1)))
		{
			varReminder = split_variable_reminder(line, i, cmd);
			break ;
		}
	}
	// Si no se pudo extraer un nombre de variable, retorna NULL.
	if (!varName)
		return (NULL);
	// Si el nombre extraído es vacío, retorna la parte de la cadena a partir del '$'.
	if (!varName[0])
		return (ft_strchr(line, '$') + 1);
	// Compara el nombre extraído con las variables de entorno y realiza la sustitución.
	return (compare_variable_name(cmd, line, varName));
}
