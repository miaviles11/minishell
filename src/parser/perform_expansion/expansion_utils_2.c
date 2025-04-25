/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:20:24 by carlsanc          #+#    #+#             */
/*   Updated: 2025/04/05 10:20:24 by carlsanc         ###   ########.fr       */
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
	if (s[i] == '$' && s[i + 1] && is_digit_special(s[i + 1]) == 1)
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
char	*quit_variable_and_digit(char *s)
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
		if (s[i] == '$' && s[i + 1] && is_digit_special(s[i + 1]) == 1)
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


/* ———————————————————————————————————————————— */
/* is_literal_dollar:
**   Determina si line es un '$' literal (sin nombre de variable).
*/
static int is_literal_dollar(const char *line)
{
    if (!line || line[0] != '$')
        return (0);
    if (line[1] == '\0')
        return (1);
    if (line[1] != '{' && line[1] != '?' && !ft_isalnum(line[1]) && line[1] != '_')
        return (1);
    return (0);
}

/* ———————————————————————————————————————————— */
/* detect_translation:
**   Si la línea empieza con $" y encuentra '"',
**   devuelve 1 y pone en *end el índice de esa '"'.
*/
static int detect_translation(const char *line, int *end)
{
    int i;

    if (!line || line[0] != '$' || line[1] != '"')
        return (0);
    i = get_next_quote(2, (char *)line, '"');
    if (line[i] != '"')
        return (0);
    *end = i;
    return (1);
}

/* ———————————————————————————————————————————— */
/* expand_translation:
**   Extrae el contenido entre $"..." y concatena con el sufijo.
*/
static char *expand_translation(t_msh *msh, char *line)
{
    char *inside;
    char *suffix;
    char *res;
    int   end;

    (void)msh;
    if (!detect_translation(line, &end))
        return (line);
    inside = ft_substr(line + 2, 0, end - 2);
    suffix = ft_strdup(line + end + 1);
    res    = ft_strjoin(inside, suffix);
    free(inside);
    free(suffix);
    free(line);
    return (res);
}

/* ———————————————————————————————————————————— */
/* parse_braced:
**   Detecta índices de ${VAR:offset} y su cierre '}'.
*/
static int parse_braced(const char *line, int *colon, int *end)
{
    int i;

    if (!line || line[0] != '$' || line[1] != '{')
        return (0);
    i = 2;
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    if (line[i] != ':')
        return (0);
    *colon = i;
    while (line[i] && line[i] != '}')
        i++;
    if (line[i] != '}')
        return (0);
    *end = i;
    return (1);
}

/* ———————————————————————————————————————————— */
/* expand_braced_substring:
**   Extrae VAR, aplica offset y concatena sufijo.
*/
static char *expand_braced_substring(t_msh *msh, char *line)
{
    char *name;
    char *value;
    char *substr;
    char *suffix;
    char *res;
    int   colon, end, offset;

    if (!parse_braced(line, &colon, &end))
        return (line);
    name   = ft_substr(line + 2, 0, colon - 2);
    offset = ft_atoi(line + colon + 1);
    suffix = ft_strdup(line + end + 1);
    value  = get_env_value(msh, name);
    free(name);
    if (!value)
        value = ft_strdup("");
    if (offset < (int)ft_strlen(value))
        substr = ft_strdup(value + offset);
    else
        substr = ft_strdup("");
    free(value);
    res  = ft_strjoin(substr, suffix);
    free(substr);
    free(suffix);
    free(line);
    return (res);
}

/* ———————————————————————————————————————————— */
/* substitute_variable_value:
**   0) Si es '$' literal, lo deja
**   1) Expande $"..."
**   2) Expande ${VAR:offset}
**   3) Maneja $?
**   4) Expansión normal de $VAR
*/
char *substitute_variable_value(t_msh *msh, t_cmd *cmd, char *line, char **varReminder)
{
    char *varName;
    char *tmp;
    int   i;

    /* 0) '$' literal */
    if (is_literal_dollar(line))
        return (line);
    /* 1) traducción $"..." */
    tmp = expand_translation(msh, line);
    if (tmp != line)
        return (tmp);
    /* 2) ${VAR:offset} */
    tmp = expand_braced_substring(msh, line);
    if (tmp != line)
        return (tmp);
    /* 3) $? */
    if (ft_strnstr(line, "$?", ft_strlen(line)))
        return (replace_special_value(line, msh->error_value));
    /* 4) $VAR normal */
    if (!cmd->flags)
    {
        cmd->flags = calloc(1, sizeof(*(cmd->flags)));
        if (!cmd->flags)
            exit_error("Error malloc flags", 12);
    }
    if (check_variable_and_digit(line) == 0)
        line = quit_variable_and_digit(line);
    varName = extract_variable_name(line);
    if (!varName)
        return (line);
    i = find_next_dollar(line, -1);
    if (i == -1)
    {
        free(varName);
        return (line);
    }
    {
        int nameIndex = i + 1;
        int nameLen   = ft_strlen(varName);
        i = nameIndex + nameLen - 1;
    }
    if (line[i + 1] &&
        ((special_char_check(line[i + 1]) == -1 &&
          line[i + 1] != '$' && line[i + 1] != '"') ||
         (line[i + 1] == '$' &&
          (i + 1) == find_next_dollar(line, i))))
    {
        if (varReminder)
        {
            *varReminder = ft_strdup(line + i + 1);
            cmd->flags->dollar_special = 1;
            line[i + 1] = '\0';
        }
    }
    return (compare_variable_name(msh, line, varName));
}
