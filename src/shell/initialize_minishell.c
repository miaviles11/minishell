/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:40:42 by miaviles          #+#    #+#             */
/*   Updated: 2025/05/02 00:02:02 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_msh *initialize_shell(char **envp)
{
    t_msh *shell;
    int i;

    shell = (t_msh *)malloc(sizeof(t_msh));
    if (!shell)
        return (NULL);

    shell->bash_lvl = 0;
    shell->total_chars = 0;
    shell->cmd = NULL;
    shell->path = NULL;
    shell->error_value = 0;
    shell->quote = 0;
    shell->pipe = 0;
    shell->redic = 0;

    shell->num_env = 0;
    if (envp)
    {
        while (envp[shell->num_env])
            shell->num_env++;
    }

    shell->env = (char **)malloc(sizeof(char *) * (shell->num_env + 1));
    if (!shell->env)
    {
        free(shell);
        return (NULL);
    }

    for (i = 0; i < shell->num_env; i++)
    {
        shell->env[i] = strdup(envp[i]);
        if (!shell->env[i])
        {
            while (--i >= 0)
                free(shell->env[i]);
            free(shell->env);
            free(shell);
            return (NULL);
        }
    }
    shell->env[shell->num_env] = NULL;

    shell->path = get_env_value(shell, "PATH");

    update_shell_level(shell);

    return (shell);
}

char *get_env_value(t_msh *shell, const char *var_name)
{
    int i;
    int len;
    
    if (!shell || !var_name || !shell->env)
        return (NULL);
    
    len = strlen(var_name);
    
    for (i = 0; i < shell->num_env; i++)
    {
        if (strncmp(shell->env[i], var_name, len) == 0 && 
            shell->env[i][len] == '=')
        {
            return (strdup(shell->env[i] + len + 1));
        }
    }
    
    return (NULL);
}

void update_shell_level(t_msh *shell)
{
    char *shlvl_str;
    int shlvl;
    char *new_shlvl;
    char *new_shlvl_entry;
    
    shlvl_str = get_env_value(shell, "SHLVL");
    
    shlvl = 1;
    if (shlvl_str)
    {
        shlvl = atoi(shlvl_str);
        free(shlvl_str);
        shlvl++;
    }
    
    shell->bash_lvl = shlvl;
    
    new_shlvl = ft_itoa(shlvl);
    if (!new_shlvl)
        return;
    
    new_shlvl_entry = ft_strjoin("SHLVL=", new_shlvl);
    free(new_shlvl);
    
    if (!new_shlvl_entry)
        return;
    
    set_env_value(shell, "SHLVL", new_shlvl_entry + 6);
    free(new_shlvl_entry);
}

int set_env_value(t_msh *shell, const char *var_name, const char *value)
{
    int i;
    int len;
    char *new_entry;
    
    if (!shell || !var_name || !value)
        return (-1);
    len = strlen(var_name);
    for (i = 0; i < shell->num_env; i++)
    {
        if (strncmp(shell->env[i], var_name, len) == 0 && 
            shell->env[i][len] == '=')
        {
            free(shell->env[i]);
            new_entry = malloc(len + strlen(value) + 2);
            if (!new_entry)
                return (-1);
            strcpy(new_entry, var_name);
            strcat(new_entry, "=");
            strcat(new_entry, value);
            shell->env[i] = new_entry;
            return (0);
        }
    }

    char **new_env = realloc(shell->env, sizeof(char*) * (shell->num_env + 2));
    if (!new_env)
        return (-1);
    
    shell->env = new_env;
    
    new_entry = malloc(len + strlen(value) + 2);
    if (!new_entry)
        return (-1);
    strcpy(new_entry, var_name);
    strcat(new_entry, "=");
    strcat(new_entry, value);
    shell->env[shell->num_env] = new_entry;
    shell->env[shell->num_env + 1] = NULL;
    shell->num_env++;
    return (0);
}
