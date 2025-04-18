/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
** is_redirect_operator:
**   Retorna 1 si el carácter 'c' es un operador de redirección ('>' o '<'),
**   o 0 en caso contrario.
*/
int	is_redirect_operator(char c)
{
	return (c == '>' || c == '<');
}

/*
** get_redirect_type:
**   Recorre la cadena 'str' (ignorando contenido entre comillas) y detecta
**   el primer operador de redirección válido. Retorna:
**     1  para '>' simple,
**     2  para '>>',
**     3  para '<' simple,
**     4  para '<<',
**     5  para '2>' (redirección de error simple),
**     6  para '2>>',
**    -1  si se detecta un operador mal formado,
**     0  si no se encuentra ningún operador.
*/
#include "../includes/minishell.h"

int	get_redirect_type(char *s)
{
	int i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			i = get_next_quote(i + 1, s, s[i]);
		if (!ft_strncmp(s + i, "<<<", 3))
			return (-1);
		if (!ft_strncmp(s + i, "2>>", 3))
			return (6);
		if (!ft_strncmp(s + i, "2>", 2) && !is_redirect_operator(s[i + 2]))
			return (5);
		if (!ft_strncmp(s + i, ">>", 2) && !is_redirect_operator(s[i + 2]))
			return (2);
		if (s[i] == '>' && !is_redirect_operator(s[i + 1]))
			return (1);
		if (!ft_strncmp(s + i, "<<", 2) && !is_redirect_operator(s[i + 2]))
			return (4);
		if (s[i] == '<' && !is_redirect_operator(s[i + 1]))
			return (3);
		if (is_redirect_operator(s[i]))
			return (-1);
		i++;
	}
	return (0);
}



/*
** find_first_redirect_index:
**   Recorre el arreglo de cadenas 'args' y retorna el índice del primer
**   elemento que contenga un operador de redirección válido (según get_redirect_type).
**   Retorna -1 si no se encuentra ninguno.
*/
int	find_first_redirect_index(char **args)
{
	int	i = 0;

	while (args[i])
	{
		if (get_redirect_type(args[i]))
			return (i);
		i++;
	}
	return (-1);
}

/*
** get_operator_for_type:
**   Dado un tipo de redirección (n), retorna el carácter base:
**     '>' para tipos 1, 2, 5 y 6;
**     '<' para tipos 3 y 4;
**     0 en caso contrario.
*/
char	get_operator_for_type(int n)
{
	if (n == 1 || n == 2 || n == 5 || n == 6)
		return ('>');
	else if (n == 3 || n == 4)
		return ('<');
	return (0);
}

/*
** find_next_redirect_operator_index:
**   A partir del índice 'i' en la cadena 'str', avanza hasta encontrar
**   el siguiente carácter que sea un operador de redirección.
**   Retorna el índice encontrado o el final de la cadena.
*/
int	find_next_redirect_operator_index(int i, char *str)
{
	while (str[i] && !is_redirect_operator(str[i]))
		i++;
	return (i);
}
