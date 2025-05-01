/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:20:24 by carlsanc          #+#    #+#             */
/*   Updated: 2025/04/05 10:20:24 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	check_variable_and_digit(const char *s)
{
	int	i = 0;

	while (s[i] && s[i] != '$')
		i++;
	if (s[i] == '$' && s[i + 1] && is_digit_special(s[i + 1]) == 1)
		return (0);
	return (1);
}

char	*quit_variable_and_digit(char *s)
{
	int		i;
	int		j;
	char	*result;
	int		len;

	len = ft_strlen(s);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		exit_error("Error malloc", 15);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] && is_digit_special(s[i + 1]) == 1)
		{
			i += 2;
			continue;
		}
		result[j++] = s[i++];
	}
	result[j] = '\0';
	free(s);
	return (result);
}

int	is_digit_special(int c)
{
	if (c == '*' || c == '@' || c == '\\')
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int is_literal_dollar(const char *line)
{
    if (!line || line[0] != '$')
        return (0);
    if (line[1] == '\0')
        return (1);
    if (line[1] != '{' && line[1] != '?' && !ft_isalnum(line[1]) && line[1] != '_')
        return (1);
    return (0);
}

static int detect_translation(const char *line, int *end)
{
    int i;

    if (!line || line[0] != '$' || line[1] != '"')
        return (0);
    i = get_next_quote(2, (char *)line, '"');
    if (line[i] != '"')
        return (0);
    *end = i;
    return (1);
}

static char *expand_translation(t_msh *msh, char *line)
{
    char *inside;
    char *suffix;
    char *res;
    int   end;

    (void)msh;
    if (!detect_translation(line, &end))
        return (line);
    inside = ft_substr(line + 2, 0, end - 2);
    suffix = ft_strdup(line + end + 1);
    res    = ft_strjoin(inside, suffix);
    free(inside);
    free(suffix);
    free(line);
    return (res);
}

static int parse_braced(const char *line, int *colon, int *end)
{
    int i;

    if (!line || line[0] != '$' || line[1] != '{')
        return (0);
    i = 2;
    while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
        i++;
    if (line[i] != ':')
        return (0);
    *colon = i;
    while (line[i] && line[i] != '}')
        i++;
    if (line[i] != '}')
        return (0);
    *end = i;
    return (1);
}

static char *expand_braced_substring(t_msh *msh, char *line)
{
    char *name;
    char *value;
    char *substr;
    char *suffix;
    char *res;
    int   colon, end, offset;

    if (!parse_braced(line, &colon, &end))
        return (line);
    name   = ft_substr(line + 2, 0, colon - 2);
    offset = ft_atoi(line + colon + 1);
    suffix = ft_strdup(line + end + 1);
    value  = get_env_value(msh, name);
    free(name);
    if (!value)
        value = ft_strdup("");
    if (offset < (int)ft_strlen(value))
        substr = ft_strdup(value + offset);
    else
        substr = ft_strdup("");
    free(value);
    res  = ft_strjoin(substr, suffix);
    free(substr);
    free(suffix);
    free(line);
    return (res);
}

char *substitute_variable_value(t_msh *msh, t_cmd *cmd, char *line, char **varReminder)
{
    char *varName;
    char *tmp;
    int   i;

    if (is_literal_dollar(line))
        return (line);
    tmp = expand_translation(msh, line);
    if (tmp != line)
        return (tmp);
    tmp = expand_braced_substring(msh, line);
    if (tmp != line)
        return (tmp);
    if (ft_strnstr(line, "$?", ft_strlen(line)))
        return (replace_special_value(line, msh->error_value));
    if (!cmd->flags)
    {
        cmd->flags = calloc(1, sizeof(*(cmd->flags)));
        if (!cmd->flags)
            exit_error("Error malloc flags", 12);
    }
    if (check_variable_and_digit(line) == 0)
        line = quit_variable_and_digit(line);
    varName = extract_variable_name(line);
    if (!varName)
        return (line);
    i = find_next_dollar(line, -1);
    if (i == -1)
    {
        free(varName);
        return (line);
    }
    {
        int nameIndex = i + 1;
        int nameLen   = ft_strlen(varName);
        i = nameIndex + nameLen - 1;
    }
    if (line[i + 1] &&
        ((special_char_check(line[i + 1]) == -1 &&
          line[i + 1] != '$' && line[i + 1] != '"') ||
         (line[i + 1] == '$' &&
          (i + 1) == find_next_dollar(line, i))))
    {
        if (varReminder)
        {
            *varReminder = ft_strdup(line + i + 1);
            cmd->flags->dollar_special = 1;
            line[i + 1] = '\0';
        }
    }
    return (compare_variable_name(msh, line, varName));
}
