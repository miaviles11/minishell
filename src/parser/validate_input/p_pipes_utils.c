/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_pipes_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
/*
** extract_pipe_segments:
**   Divide la cadena 's' en segmentos utilizando el carácter pipe ('|')
**   como delimitador, ignorando los pipes que aparezcan dentro de comillas.
**   Los segmentos se almacenan en el arreglo 'segments', que debe estar
**   previamente reservado con suficiente espacio (número de pipes + 2).
**   Retorna 'segments'.
*/
char	**extract_pipe_segments(char *s, char **segments)
{
	int	i = 0, start = 0, pos = 0;

	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			i = get_next_quote(i + 1, s, s[i]);
		if ((s[i + 1] == '|' || s[i + 1] == '\0') && s[i] != '|')
		{
			segments[pos] = ft_substr(s, start, i - start + 1);
			if (!segments[pos])
				exit_error("Error malloc", 54);
			pos++;
		}
		if (s[i] == '|' && !(s[i + 1] == '|' || s[i + 1] == '\0'))
			start = i + 1;
		i++;
	}
	segments[pos] = NULL;
	return (segments);
}

/*
** validate_pipe_segment:
**   Verifica que el segmento 'segment' no esté compuesto únicamente por
**   espacios en blanco (lo que indicaría un error de sintaxis).
**   Retorna 0 si es válido, o -1 si sólo contiene espacios.
*/
int	validate_pipe_segment(char *segment)
{
	int	i = 0;

	while (segment[i])
	{
		if (segment[i] != ' ' && segment[i] != '\t' && segment[i] != '\n')
			return (0);
		i++;
	}
	put_error("bash", NULL, "syntax error near unexpected token `|'");
	return (-1);
}

/*
** validate_redirection_syntax:
**   Verifica que el segmento segments[index] tenga una sintaxis válida en cuanto
**   a redirecciones. Para ello, utiliza get_redirect_type() para identificar el
**   operador y get_operator_for_type() para localizarlo en el segmento.
**
**   Si se detecta un error (por ejemplo, un operador sin operando o mal formado),
**   se muestra un mensaje de error y se retorna -1. En caso contrario, retorna 0.
*/
int	validate_redirection_syntax(char **segments, int index)
{
	char	*temp;
	int		j = 0;

	if (get_redirect_type(segments[index]) == -1)
	{
		put_error("bash", NULL, "syntax error near unexpected token `>'");
		return (-1);
	}
	temp = ft_strchr(segments[index],
			get_operator_for_type(get_redirect_type(segments[index])));
	while (temp && get_redirect_type(temp + j))
	{
		j = 0;
		while (temp[j] && (temp[j] == ' ' || is_redirect_operator(temp[j])))
			j++;
		if ((get_redirect_type(segments[index]) && !temp[j])
			|| is_redirect_operator(temp[ft_strlen(temp) - 1]))
		{
			put_error("bash", NULL, "syntax error near unexpected token `>'");
			return (-1);
		}
		temp = ft_strchr(temp + j,
				get_operator_for_type(get_redirect_type(temp + j)));
	}
	return (0);
}
