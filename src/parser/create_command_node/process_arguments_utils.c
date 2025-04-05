/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_arguments_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:08:12 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/05 11:08:12 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*quit_null_space(char *s, int index)
{
	char	*new_str;
	int		i;
	int		j;

	new_str = malloc(ft_strlen(s) - 2 + 1); // Elimina dos caracteres (las comillas consecutivas)
	if (!new_str)
		exit_error("Error malloc", 15);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i == index || i == index + 1) // Salta las comillas consecutivas
			i++;
		else
			new_str[j++] = s[i++];
	}
	new_str[j] = '\0';
	free(s);
	return (new_str);
}

char	*quit_null(char *s, int index)
{
    char	*new_str;
    int		i;
    int		j;

    new_str = malloc(ft_strlen(s) - 2 + 1); // Elimina dos caracteres (las comillas consecutivas)
    if (!new_str)
        exit_error("Error malloc", 16);
    i = 0;
    j = 0;
    while (s[i])
    {
        if (i == index || i == index + 1) // Salta las comillas consecutivas
            i++;
        else
            new_str[j++] = s[i++];
    }
    new_str[j] = '\0';
    free(s);
    return (new_str);
}
