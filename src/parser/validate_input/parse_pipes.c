/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** skip_quoted_segment:
**   Avanza el índice 'i' en la cadena 's' para saltar el contenido delimitado
**   por la comilla 'quote'. Se asume que s[i] es la comilla de apertura.
**   Retorna el nuevo valor de i, que apunta justo después de la comilla de cierre.
*/
static int	skip_quoted_segment(const char *s, int i, char quote)
{
	i++;  // Salta la comilla de apertura
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;  // Salta la comilla de cierre
	return (i);
}

/* 
** count_valid_pipes:
**   Recorre la cadena 'inputLine' ignorando el contenido entre comillas,
**   valida la sintaxis de los pipes y cuenta los pipes válidos.
**   Retorna el número de pipes encontrados o -1 en caso de error.
*/
static int	count_valid_pipes(const char *inputLine, t_msh *msh)
{
    int	i = 0;
    int	pipeCount = 0;

    while (inputLine[i])
    {
        if (inputLine[i] == '"' || inputLine[i] == '\'')
        {
            i = skip_quoted_segment(inputLine, i, inputLine[i]);
            continue;
        }
        if (inputLine[i] == '|')
        {
            if (inputLine[i + 1] == '|' || inputLine[i + 1] == '\0')
            {
                put_error("bash", NULL, "syntax error near unexpected token `|'");
                msh->error_value = 258; // Usar msh->error_value
                return (-1);
            }
            pipeCount++;
        }
        i++;
    }
    return (pipeCount);
}

/*
** count_pipes:
**   Verifica que la línea no comience con un pipe y luego cuenta los pipes válidos.
**   Retorna el número de pipes o -1 si ocurre un error de sintaxis.
*/
int	count_pipes(const char *inputLine, t_msh *msh)
{
    if (inputLine[0] == '|')
    {
        put_error("bash", NULL, "syntax error near unexpected token `|'");
        msh->error_value = 258; // Usar msh->error_value
        return (-1);
    }
    return (count_valid_pipes(inputLine, msh));
}

char	**split_pipes(char *inputLine, t_msh *msh)
{
    char	**segments;
    int		i;

    segments = ft_calloc(count_pipes(inputLine, msh) + 2, sizeof(char *));
    if (!segments)
        exit_error("Error malloc", 54);
    segments = extract_pipe_segments(inputLine, segments);
    i = 0;
    while (segments[i])
    {
        if (validate_pipe_segment(segments[i]) == -1 ||
            validate_redirection_syntax(segments, i) == -1)
        {
            msh->error_value = 258; // Usar msh->error_value
            while (i >= 0)
                free(segments[i--]);
            free(segments);
            return (NULL);
        }
        i++;
    }
    return (segments);
}
