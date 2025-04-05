/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:23:54 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/05 10:23:54 by miaviles         ###   ########.fr       */
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
char	*substitute_variables(t_cmd *cmd, char *s, char **varReminder)
{
	char	*temp;

	if (check_variable_and_digit(s) == 0)
		s = quit_variable_and_digit(s, -1, -1);
	// Mientras se detecte una variable en la cadena...
	while (has_variable(s))
	{
		cmd->flags->dollar_special = 0;
		s = substitute_variable_value(cmd, s, varReminder);
		// Si se activó un caso especial, concatena varReminder.
		if (cmd->flags->dollar_special == 1)
		{
			temp = join_special(s, *varReminder);
			free(s);
			s = temp;
			free(varReminder);
			varReminder = NULL;
			cmd->flags->dollar_special = 0;
		}
	}
	if (varReminder)
		free(varReminder);
	return (s);
}

/*
** join_special:
**   Concatena dos cadenas, s1 y s2, pero si s1 termina en una comilla (simple o doble),
**   esa comilla final se descarta antes de la concatenación.
**
** Retorna:
**   Una nueva cadena que es la unión de s1 (sin la comilla final, si existiera)
**   y s2. Se espera que ambas cadenas estén correctamente terminadas.
*/
char	*join_special(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;
	size_t	i, j;

	len1 = ft_strlen(s1);
	// Si s1 termina en comilla doble (") o comilla simple ('), descarta ese carácter.
	if (len1 > 0 && (s1[len1 - 1] == '"' || s1[len1 - 1] == '\''))
		len1--;
	len2 = ft_strlen(s2);
	// Reserva memoria para la cadena resultante (len1 + len2 + 1 para el '\0').
	result = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!result)
		exit_error("Error malloc", 51);
	// Copia s1 (hasta len1) en result.
	i = 0;
	while (i < len1)
	{
		result[i] = s1[i];
		i++;
	}
	// Copia s2 a continuación.
	j = 0;
	while (j < len2)
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}
/*
** needs_home_expansion:
**   Retorna 1 si la cadena 's' comienza con '~' y el siguiente carácter es '/' o es el fin de la cadena.
**   Esto indica que se debe reemplazar '~' por el directorio HOME.
**   En caso contrario, retorna 0.
*/
int	needs_home_expansion(const char *s)
{
	if (s && s[0] == '~' && (!s[1] || s[1] == '/'))
		return (1);
	return (0);
}

/*
** expand_home_directory:
**   Reemplaza el carácter '~' al inicio de la cadena 's' por el valor de la variable de entorno HOME.
**   Se asume que 's' comienza con '~'. Libera la cadena original y retorna la nueva cadena.
*/
char	*expand_home_directory(char *s)
{
	char	*home;
	char	*rest;
	char	*result;

	// Obtiene el resto de la cadena, omitiendo el '~'
	rest = ft_strdup(s + 1);
	// Obtiene el valor de HOME (se duplica para garantizar que se pueda liberar luego)
	home = ft_strdup(getenv("HOME"));
	if (!home)
		exit_error("Error malloc", 50);
	// Une el directorio HOME con el resto de la cadena
	result = ft_strjoin(home, rest);
	free(home);
	free(rest);
	free(s);
	return (result);
}
