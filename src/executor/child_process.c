/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:44:22 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/14 19:44:22 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Busca el ejecutable del comando en los directorios del PATH */
char	*find_executable(char *cmd)
{
    char	*path_env;
    char	**paths;
    char	*full_path;
    char	*temp;
    int		i;

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    i = 0;
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

/* Prepara los argumentos para execve */
static char	**prepare_argv(t_cmd *cmd)
{
    char	**argv;
    int		i;
    int		j;

    i = 0;
    j = -1;
    if (cmd->arg)
    {
        while (cmd->arg[i])
            i++;
    }
    argv = (char **)malloc(sizeof(char *) * (i + 2));
    if (!argv)
        return (NULL);
    argv[0] = cmd->cmd;
    while (++j < i)
        argv[j + 1] = cmd->arg[j];
    argv[i + 1] = NULL;
    return (argv);
}

/* Redirige entrada y salida según los descriptores de archivo */
static void	setup_redirections(int input_fd, int output_fd)
{
    if (input_fd != STDIN_FILENO)
    {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != STDOUT_FILENO)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
}

/* Ejecuta un comando en un proceso hijo con los descriptores especificados */
void	child_process(t_msh *msh, t_cmd *cmd, int input_fd, int output_fd)
{
    char	*executable;
    char	**argv;

    setup_redirections(input_fd, output_fd);
    if (msh->redic && cmd->arg && find_first_redirect_index(cmd->arg) != -1)
        process_redirections(cmd);
    executable = find_executable(cmd->cmd);
    if (!executable)
    {
        ft_printf("Command not found: %s\n", cmd->cmd);
        _exit(127);
    }
    argv = prepare_argv(cmd);
    if (!argv)
    {
        free(executable);
        _exit(1);
    }
    execve(executable, argv, cmd->env);
    perror("execve");
    free(executable);
    free(argv);
    _exit(1);
}
