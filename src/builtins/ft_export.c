/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:10:52 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 19:25:14 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char    **ft_split_sort_env(char **env)
{
    char    **sorted;
    char    *tmp;
    int      len;
    int      i;
    int      j;

    if (!env)
        return (NULL);
    len = 0;
    while (env[len])
        len++;
    sorted = malloc(sizeof(char *) * (len + 1));
    if (!sorted)
        exit_error("Error malloc", 1);
    i = 0;
    while (i < len)
    {
        sorted[i] = ft_strdup(env[i]);
        if (!sorted[i])
            exit_error("Error malloc", 1);
        i++;
    }
    sorted[len] = NULL;
    i = 0;
    while (i < len - 1)
    {
        j = i + 1;
        while (j < len)
        {
            if (ft_strcmp(sorted[i], sorted[j]) > 0)
            {
                tmp          = sorted[i];
                sorted[i]    = sorted[j];
                sorted[j]    = tmp;
            }
            j++;
        }
        i++;
    }
    return (sorted);
}

void	handle_no_equal(t_msh *msh, const char *arg)
{
	char *key;
	char *existing_value;

	key = ft_strdup(arg);
	if (!key)
		return;

	existing_value = getenv(key);
	if (existing_value)
		set_env_var(msh, key, existing_value);
	else
		set_env_var(msh, key, "");
	free(key);
}

void	update_env(t_msh *msh, const char *arg)
{
	char *key;
	char *value;
	char *equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
	{
		handle_no_equal(msh, arg);
		return;
	}
	key = ft_substr(arg, 0, equal_pos - arg);
	value = ft_strdup(equal_pos + 1);

	if (!key || !value)
	{
		free(key);
		free(value);
		return;
	}
	set_env_var(msh, key, value);
	free(key);
	free(value);
}

int minishell_export(t_msh *msh, char **argv)
{
    int i;

    if (!argv[1])
    {
        char **sorted = ft_split_sort_env(msh->env);
        int  j = 0;

        while (sorted[j])
        {
            char *eq = ft_strchr(sorted[j], '=');
            if (eq)
            {
                *eq = '\0';
                printf("declare -x %s=\"%s\"\n", sorted[j], eq + 1);
                *eq = '=';
            }
            else
            {
                printf("declare -x %s\n", sorted[j]);
            }
            j++;
        }
        ft_free_split(sorted);
        return (0);
    }
    i = 1;
    while (argv[i])
    {
        if (is_valid_identifier(argv[i]))
            update_env(msh, argv[i]);
        else
            ft_printf("export: `%s': not a valid identifier\n", argv[i]);
        i++;
    }
    return (0);
}
