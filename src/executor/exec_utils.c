/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:19:08 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/17 16:25:01 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Espera a que terminen todos los procesos hijos pendientes*/

void	wait_for_children(void)
{
	pid_t	wpid;
	int		status;

	while ((wpid = wait(&status)) > 0)
		;
}

char	*check_command_path(char *cmd)
{
    // Si es una ruta absoluta o relativa
    if (cmd[0] == '/' || cmd[0] == '.' || ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
    }
    return (NULL);
}

/* Busca el ejecutable en los directorios del PATH */
char	*search_in_path(char *cmd)
{
    char *path_env;
    char **paths;
    char *full_path;
    char *temp;
    int i = 0;

    if (!(path_env = getenv("PATH")) || !(paths = ft_split(path_env, ':')))
        return (NULL);
    while (paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        temp = full_path;
        full_path = ft_strjoin(full_path, cmd);
        free(temp);
        if (access(full_path, X_OK) == 0)
        {
            ft_free_split(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    ft_free_split(paths);
    return (NULL);
}
