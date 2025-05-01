/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:23:41 by carlsanc          #+#    #+#             */
/*   Updated: 2025/04/05 10:23:41 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char *replace_special_value(char *s, int error_value)
{
    int i;
    int j;

    i = get_next_quote(0, s, '?') - 1;
    while (s[i])
    {
        if (s[i] == '$' && s[i + 1] && s[i + 1] == '?')
            s = replace_special_value_helper(s, i, error_value);
        j = i;
        i = get_next_quote(0, s, '$');
        if (j == i)
            break;
    }
    return (s);
}
char *replace_special_value_helper(char *s, int i, int error_value)
{
    char *errorStr;
    char *prefix;
    char *suffix;
    char *result;
    char *temp;

    errorStr = ft_itoa(error_value);
    prefix = ft_substr(s, 0, i);
    suffix = ft_strdup(s + i + 2);
    result = ft_strjoin(prefix, errorStr);
    temp = result;
    result = ft_strjoin(result, suffix);
    free(temp);
    free(prefix);
    free(suffix);
    free(errorStr);
    free(s);
    return (result);
}

char	*extract_variable_name(char *line)
{
	int		i;
	int		length;
	char	*result;

	i = get_next_quote(0, line, '$') + 1;
	length = 0;
	while (line[i + length] && line[i + length] != ' ' &&
		   line[i + length] != '"' && line[i + length] != '\'' &&
		   special_char_check(line[i + length]) == 0)
		length++;
	result = ft_calloc(length + 1, sizeof(char));
	if (!result)
		exit_error("Error malloc", 16);
	length = 0;
	while (line[i + length] && line[i + length] != ' ' &&
		   line[i + length] != '"' && line[i + length] != '\'' &&
		   special_char_check(line[i + length]) == 0)
	{
		result[length] = line[i + length];
		length++;
	}
	return (result);
}

int find_next_dollar(const char *s, int start)
{
    int i = (start < 0 ? 0 : start);
    int in_dq = 0;
    int in_sq = 0;

    while (s[i])
    {
        if (s[i] == '"' && !in_sq)
            in_dq = !in_dq;
        else if (s[i] == '\'' && !in_dq)
            in_sq = !in_sq;
        else if (s[i] == '$' && !in_sq)
            return i;
        i++;
    }
    return -1;
}

char	*split_variable_reminder(char *line, int index, t_cmd *cmd)
{
	int		len;
	int		i;
	char	*res;

	cmd->flags->dollar_special = 1;
	
	len = 0;
	while (line[index + len])
		len++;
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		exit_error("Error malloc", 17);
	i = 0;
	while (i < len)
	{
		res[i] = line[index + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
