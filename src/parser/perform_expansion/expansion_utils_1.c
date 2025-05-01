/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:23:54 by carlsanc          #+#    #+#             */
/*   Updated: 2025/04/05 10:23:54 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int has_variable(const char *s)
{
    int i = 0;
    int in_dq = 0;
    int in_sq = 0;

    while (s[i])
    {
        if (s[i] == '"' && !in_sq)
            in_dq = !in_dq;
        else if (s[i] == '\'' && !in_dq)
            in_sq = !in_sq;
        else if (s[i] == '$' && !in_sq && s[i+1] && s[i+1] != ' ')
            return 1;
        i++;
    }
    return 0;
}

char	*substitute_variables(t_msh *msh, t_cmd *cmd, char *s, char **varReminder)
{
	char	*temp;

	if (!cmd->flags)
	{
        cmd->flags = calloc(1, sizeof(*(cmd->flags)));
        if (!cmd->flags)
            exit_error("Error malloc flags", 12);
    }
	if (check_variable_and_digit(s) == 0)
		s = quit_variable_and_digit(s);
	char *prev_s = NULL;
	while (has_variable(s))
	{
		cmd->flags->dollar_special = 0;
		prev_s = ft_strdup(s);
		s = substitute_variable_value(msh, cmd, s, varReminder);
		if (cmd->flags->dollar_special == 1 && varReminder && *varReminder)
		{
			temp = join_special(s, *varReminder);
			free(s);
			s = temp;
			free(*varReminder);
			*varReminder = NULL;
			cmd->flags->dollar_special = 0;
		}
		if (ft_strcmp(prev_s, s) == 0)
		{
			free(prev_s);
			break;
		}
		free(prev_s);
	}
	return (s);
}

char	*join_special(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;
	size_t	i, j;

	len1 = ft_strlen(s1);
	if (len1 > 0 && (s1[len1 - 1] == '"' || s1[len1 - 1] == '\''))
		len1--;
	len2 = ft_strlen(s2);
	result = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!result)
		exit_error("Error malloc", 51);
	i = 0;
	while (i < len1)
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}

int	needs_home_expansion(const char *s)
{
	if (s && s[0] == '~' && (!s[1] || s[1] == '/'))
		return (1);
	return (0);
}

char	*expand_home_directory(char *s)
{
	char	*home;
	char	*rest;
	char	*result;

	rest = ft_strdup(s + 1);
	home = ft_strdup(getenv("HOME"));
	if (!home)
		exit_error("Error malloc", 50);
	result = ft_strjoin(home, rest);
	free(home);
	free(rest);
	free(s);
	return (result);
}
