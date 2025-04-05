/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:23:28 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/05 10:23:28 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
/*
** special_char_check:
**   Determina si el carácter 'c' es considerado un carácter especial,
**   que no es permitido en los nombres de variables u otros identificadores.
**   Retorna -1 si 'c' es especial; de lo contrario, retorna 0.
*/
int	special_char_check(char c)
{
	if (c == '=' || c == '@' || c == '#' || c == '-' || c == '+' ||
	    c == '{' || c == '}' || c == '[' || c == ']' || c == '!' ||
	    c == '~' || c == '?' || c == '%' || c == '^' || c == '*' ||
	    c == '/' || c == '$' || c == ';')
		return (-1);
	return (0);
}
/*
** compare_variable_name:
**   Compara el nombre extraído de la variable (nameVar) con cada variable
**   de entorno almacenada en msh->env. Se utiliza var_strcmp para comparar
**   el nombre extraído con la parte del nombre de cada variable de entorno.
**
**   Si se encuentra una coincidencia, se libera nameVar, se extrae el valor de
**   la variable (usando ft_add_var_value) y se llama a replace_variable_in_line para
**   reemplazar la parte correspondiente en la línea.
**
**   Si no se encuentra ninguna coincidencia, se libera nameVar y se reemplaza
**   por una cadena vacía en la línea.
**
** Retorna:
**   La línea modificada con la variable sustituida, o el resultado de replace_variable_in_line.
*/
char	*compare_variable_name(t_msh *msh, char *line, char *nameVar)
{
    int		envIndex;
    char	*varValue;

    envIndex = 0;
    while (envIndex < msh->num_env)
    {
        if (compare_env_variable_name(nameVar, msh->env[envIndex]) == 0)
        {
            free(nameVar);
            varValue = extract_env_value(msh->env[envIndex]);
            return (replace_variable_in_line(line, varValue));
        }
        envIndex++;
    }
    free(nameVar);
    return (replace_variable_in_line(line, ft_strdup("")));
}
/*
** compare_env_variable_name:
**   Compara el nombre de la variable almacenado en s1 con la parte de nombre de
**   la variable de entorno en s2 (que se asume tiene el formato "NAME=VALUE").
**
**   Retorna 0 si el nombre en s1 es idéntico al nombre en s2 (hasta el signo '=' en s2),
**   o 1 si son diferentes o si ocurre algún error.
*/
int	compare_env_variable_name(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	// Recorre s2 hasta encontrar '=' o el final de la cadena
	while (s2[i] && s2[i] != '=')
		i++;
	// Si la longitud de s1 difiere de la parte de s2 hasta '=', son distintos.
	if (ft_strlen(s1) != i)
		return (1);
	i = 0;
	// Compara carácter a carácter hasta llegar al '=' o fin de cadena
	while (s1[i] && s2[i] && s2[i] != '=')
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	return (0);
}
/*
** extract_env_value:
**   Extrae y retorna el valor de la variable de entorno contenida en la cadena
**   'envVar', que se asume tiene el formato "NAME=VALUE".
**   Si no se encuentra el carácter '=', retorna una cadena vacía.
**
** Ejemplo:
**   Input : "PATH=/usr/bin:/bin"
**   Output: "/usr/bin:/bin"
*/
char	*extract_env_value(const char *envVar)
{
	int		i;
	int		len;
	char	*result;

	i = 0;
	// Busca el carácter '=' en la cadena.
	while (envVar[i] && envVar[i] != '=')
		i++;
	// Si no se encontró '=', retorna una cadena vacía.
	if (!envVar[i])
		return (ft_strdup(""));
	// Avanza para saltar el '='.
	i++;
	// Calcula la longitud del valor.
	len = ft_strlen(envVar) - i;
	// Reserva memoria para el valor más el terminador nulo.
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		exit_error("Error malloc", 17);
	// Copia el valor desde la posición i hasta el final.
	ft_strlcpy(result, envVar + i, len + 1);
	return (result);
}
/*
** replace_variable_in_line:
**   Reemplaza en la cadena 'line' el primer patrón de variable (iniciado por '$')
**   por el valor de 'var'. La función:
**     1. Encuentra el primer '$' fuera de comillas (usando find_next_dollar).
**     2. Determina la longitud del nombre de la variable (hasta un espacio o comilla).
**     3. Construye una nueva cadena que consiste en:
**         - La parte de 'line' antes del '$'
**         - El contenido de 'var'
**         - La parte de 'line' posterior al nombre de la variable
**
**   Libera la cadena original 'line' y la cadena 'var', y retorna la nueva cadena.
*/
char	*replace_variable_in_line(char *line, char *var)
{
	int		posDollar;
	int		varNameLen;
	int		totalLen;
	char	*newLine;

	// Encuentra el primer '$' en la línea (se asume que find_next_dollar retorna -1 si no hay)
	posDollar = find_next_dollar(line, 0);
	if (posDollar == -1)
		return (line);
	// Calcula la longitud del nombre de la variable después del '$'
	varNameLen = 0;
	while (line[posDollar + 1 + varNameLen] &&
		   line[posDollar + 1 + varNameLen] != ' ' &&
		   line[posDollar + 1 + varNameLen] != '"' &&
		   line[posDollar + 1 + varNameLen] != '\'')
		varNameLen++;
	// Calcula la longitud total para la nueva cadena:
	// parte antes del '$' + longitud del valor de la variable + parte después del nombre
	totalLen = posDollar + ft_strlen(var) +
		ft_strlen(line + posDollar + 1 + varNameLen) + 1;
	newLine = malloc(sizeof(char) * totalLen);
	if (!newLine)
		exit_error("Error malloc", 18);
	// Copia la parte de la línea antes del '$'
	ft_strlcpy(newLine, line, posDollar + 1);
	// Concatena el valor de la variable
	ft_strlcat(newLine, var, totalLen);
	// Concatena la parte restante de la línea, después del nombre de la variable
	ft_strlcat(newLine, line + posDollar + 1 + varNameLen, totalLen);
	free(line);
	free(var);
	return (newLine);
}
