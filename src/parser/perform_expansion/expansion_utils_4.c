/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:23:28 by carlsanc          #+#    #+#             */
/*   Updated: 2025/04/05 10:23:28 by carlsanc         ###   ########.fr       */
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
    if ((c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == '_')
    {
        return (0);
    }
    return (-1);
}

/*
** compare_variable_name:
**   Compara el nombre extraído de la variable (nameVar) con cada variable
**   de entorno almacenada en msh->env. Se utiliza var_strcmp para comparar
**   el nombre extraído con la parte del nombre de cada variable de entorno.
**
**   Si se encuentra una coincidencia, se libera nameVar, se extrae el valor de
**   la variable (usando extract_env_value) y se llama a replace_variable_in_line para
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
    int envIndex;
    char *varValue;

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
    size_t i;

    if (!s1 || !s2)
        return (1);
    i = 0;
    while (s2[i] && s2[i] != '=')
        i++;
    if (ft_strlen(s1) != i)
        return (1);
    i = 0;
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
    int i;
    int len;
    char *result;

    i = 0;
    while (envVar[i] && envVar[i] != '=')
        i++;
    if (!envVar[i])
        return (ft_strdup(""));
    i++;
    len = ft_strlen(envVar) - i;
    result = malloc(len + 1);
    if (!result)
        exit_error("Error malloc", 17);
    ft_strlcpy(result, envVar + i, len + 1);
    return (result);
}

/*
** replace_variable_in_line:
**   Reemplaza en la cadena 'line' el primer patrón de variable (iniciado por '$')
**   por el valor de 'var'. La función:
**     1. Encuentra el primer '$' fuera de comillas (usando find_next_dollar).
**     2. Determina la longitud del nombre de la variable (hasta un espacio o comilla).
**     3. Construye una nueva cadena:
**         - Parte antes del '$'
**         - Contenido de 'var'
**         - Parte después del nombre de la variable
**   Libera la cadena original 'line' y 'var', y retorna la nueva cadena.
*/
char *replace_variable_in_line(char *line, char *var)
{
    int pos;
    int name_len;
    int new_len;
    char *new_line;

    pos = find_next_dollar(line, 0);
    if (pos < 0)
        return (line);
    /* Saltar '$' */
    name_len = 1;
    while (line[pos + name_len] && special_char_check(line[pos + name_len]) == 0)
        name_len++;
    /* Calcula nuevo tamaño: antes de '$', valor var, resto después de nombre */
    new_len = pos + ft_strlen(var) + ft_strlen(line + pos + name_len) + 1;
    new_line = malloc(new_len);
    if (!new_line)
        exit_error("Error malloc", 18);
    /* Copia parte antes de '$' */
    ft_strlcpy(new_line, line, pos + 1);
    /* Concatena valor */
    ft_strlcat(new_line, var, new_len);
    /* Concatena resto */
    ft_strlcat(new_line, line + pos + name_len, new_len);
    free(line);
    free(var);
    return (new_line);
}

