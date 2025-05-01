/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:23:28 by carlsanc          #+#    #+#             */
/*   Updated: 2025/04/05 10:23:28 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	special_char_check(char c)
{
    if ((c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == '_')
    {
        return (0);
    }
    return (-1);
}

char	*compare_variable_name(t_msh *msh, char *line, char *nameVar)
{
    int envIndex;
    char *varValue;

    envIndex = 0;
    while (envIndex < msh->num_env)
    {
        if (compare_env_variable_name(nameVar, msh->env[envIndex]) == 0)
        {
            free(nameVar);
            varValue = extract_env_value(msh->env[envIndex]);
            return (replace_variable_in_line(line, varValue));
        }
        envIndex++;
    }
    free(nameVar);
    return (replace_variable_in_line(line, ft_strdup("")));
}

int	compare_env_variable_name(const char *s1, const char *s2)
{
    size_t i;

    if (!s1 || !s2)
        return (1);
    i = 0;
    while (s2[i] && s2[i] != '=')
        i++;
    if (ft_strlen(s1) != i)
        return (1);
    i = 0;
    while (s1[i] && s2[i] && s2[i] != '=')
    {
        if (s1[i] != s2[i])
            return (1);
        i++;
    }
    return (0);
}

char	*extract_env_value(const char *envVar)
{
    int i;
    int len;
    char *result;

    i = 0;
    while (envVar[i] && envVar[i] != '=')
        i++;
    if (!envVar[i])
        return (ft_strdup(""));
    i++;
    len = ft_strlen(envVar) - i;
    result = malloc(len + 1);
    if (!result)
        exit_error("Error malloc", 17);
    ft_strlcpy(result, envVar + i, len + 1);
    return (result);
}

char *replace_variable_in_line(char *line, char *var)
{
    int pos;
    int name_len;
    int new_len;
    char *new_line;

    pos = find_next_dollar(line, 0);
    if (pos < 0)
        return (line);
    name_len = 1;
    while (line[pos + name_len] && special_char_check(line[pos + name_len]) == 0)
        name_len++;
    new_len = pos + ft_strlen(var) + ft_strlen(line + pos + name_len) + 1;
    new_line = malloc(new_len);
    if (!new_line)
        exit_error("Error malloc", 18);
    ft_strlcpy(new_line, line, pos + 1);
    ft_strlcat(new_line, var, new_len);
    ft_strlcat(new_line, line + pos + name_len, new_len);
    free(line);
    free(var);
    return (new_line);
}

