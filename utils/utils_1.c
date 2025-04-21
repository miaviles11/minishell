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

#include "../includes/minishell.h"

int is_line_empty(const char *s)
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

/*char *str_noquotes_expand(char *str, t_msh *msh)
{
	int i = 0;
	int j = 0;
	char *temp;
	int in_dquote = 0;
	int in_squote = 0;
	char *expanded_str = NULL;

	// Expandir variables si no está completamente entre comillas simples
	if (!(str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
	{
		expanded_str = substitute_variables(msh, msh->cmd, str, NULL);
		if (expanded_str)
			str = expanded_str;
	}

	// Quitar comillas
	temp = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!temp)
		exit_error("Error malloc", 11);
	while (str[i])
	{
		if (str[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else
			temp[j++] = str[i];
		i++;
	}
	if (expanded_str && expanded_str != str)
		free(expanded_str);
	return (temp);
}*/