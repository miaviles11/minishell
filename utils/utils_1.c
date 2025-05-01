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

    if (!s || s[0] == '\0')
        return 1;

    while (s[i])
    {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' ||
            s[i] == '\v' || s[i] == '\f' || s[i] == '\r')
            whitespace_count++;
        i++;
    }
    
    if (whitespace_count == i)
        return 1;
    
    return 0;
}

char	*str_noquotes(char *str)
{
	int		i;
	int		j;
	char	*temp;
	int		in_dquote;
	int		in_squote;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
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
	return (temp);
}
