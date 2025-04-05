/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_line_empty(char *s)
{
    int i = 0;
    int whitespace_count = 0;

    /* Si la cadena es NULL o está vacía, se considera vacía */
    if (!s || s[0] == '\0')
        return 1;

    /* Se recorre la cadena contando los caracteres en blanco */
    while (s[i])
    {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' ||
            s[i] == '\v' || s[i] == '\f' || s[i] == '\r')
            whitespace_count++;
        i++;
    }
    
    /* Si todos los caracteres son espacios en blanco, la línea está vacía */
    if (whitespace_count == i)
        return 1;
    
    return 0;
}

int is_builtin_no_pipeline(t_cmd *cmd, char *cmd_name)
{
    /* Si no se proporcionó un nombre de comando, se asume que no es built-in */
    if (!cmd_name)
        return 0;
    
    /* Se comparan los nombres con los built-ins que afectan al entorno.
       Se utiliza ft_strncmp */
    if (ft_strncmp(cmd_name, "cd", 3) == 0)
        return 1;
    else if (ft_strncmp(cmd_name, "exit", 5) == 0)
        return 1;
    else if (ft_strncmp(cmd_name, "export", 7) == 0)
        return 1;
    else if (ft_strncmp(cmd_name, "unset", 6) == 0)
        return 1;
    
    /* Otros comandos, incluso si son built-in (como echo, env, pwd),
       pueden ejecutarse en un pipeline */
    return 0;
}

char	*str_noquotes(char *str)
{
	int		i;          // índice para recorrer la cadena original
	int		j;          // índice para la cadena resultante
	char	*temp;      // cadena sin comillas
	int		in_dquote;  // bandera: 1 si estamos dentro de comillas dobles
	int		in_squote;  // bandera: 1 si estamos dentro de comillas simples

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	// Reserva memoria para la cadena resultante
	temp = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!temp)
		exit_error("Error malloc", 11);
	// Recorre cada carácter de la cadena original
	while (str[i])
	{
		// Si encontramos una comilla doble y no estamos en comillas simples, alterna in_dquote
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		// Si encontramos una comilla simple y no estamos en comillas dobles, alterna in_squote
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		// Si no es una comilla delimitadora, copia el carácter
		else
			temp[j++] = str[i];
		i++;
	}
	return (temp);
}
