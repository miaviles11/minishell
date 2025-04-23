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
** count_redirections:
**   Cuenta el número de operadores de redirección en la cadena,
**   incluyendo >, >>, <, <<, 2>, 2>>.
*/
int count_redirections(const char *s)
{
    int count = 0;
    int i = 0;

    while (s[i])
    {
        if (s[i] == '"' || s[i] == '\'')
            i = get_next_quote(i + 1, (char *)s, s[i]);
        else if (s[i] == '2' && s[i + 1] == '>')
        {
            count++;
            i += (s[i + 2] == '>') ? 3 : 2;
        }
        else if ((s[i] == '>' && s[i + 1] == '>') ||
                 (s[i] == '<' && s[i + 1] == '<'))
        {
            count++;
            i += 2;
        }
        else if (s[i] == '>' || s[i] == '<')
        {
            count++;
            i++;
        }
        else
            i++;
    }
    return (count);
}

/*
** extract_redirect_token:
**   Extrae la secuencia de operadores de redirección consecutivos
**   (por ejemplo, ">>>" o "<<<") para el mensaje de error.
*/
char *extract_redirect_token(char *s)
{
    int  i = 0;
    int  start;
    char op;

    while (s[i] && !is_redirect_operator(s[i]))
        i++;
    if (!s[i])
        return (ft_strdup(""));
    op = s[i];
    start = i;
    while (s[i] == op)
        i++;
    return (ft_substr(s, start, i - start));
}

char **extract_pipe_segments(char *s, char **segments)
{
    int i = 0;
    int start = 0;
    int pos = 0;

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
        if (s[i] == '|' && s[i + 1] && s[i + 1] != '|')
            start = i + 1;
        i++;
    }
    segments[pos] = NULL;
    return (segments);
}

int validate_pipe_segment(char *segment)
{
    int i = 0;

    while (segment[i])
    {
        if (segment[i] != ' ' && segment[i] != '\t' &&
            segment[i] != '\n')
            return (0);
        i++;
    }
    put_error("bash", NULL, "syntax error near unexpected token `|' ");
    return (-1);
}

int	validate_redirection_syntax(char **segments, int index)
{
    char *segment = segments[index];
    int  rt      = get_redirect_type(segment);
    char *token;
    char *tmp;
    char *msg;

    /* Caso de operador mal formado (<<<, >|, etc.) */
    if (rt == -1)
    {
        token = extract_redirect_token(segment);
        tmp   = ft_strjoin("syntax error near unexpected token `", token);
        msg   = ft_strjoin(tmp, "`");
        put_error("bash", NULL, msg);
        free(token);
        free(tmp);
        free(msg);
        return (-1);
    }
    return (0);
}
