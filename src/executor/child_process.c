/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 19:44:22 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/17 16:24:35 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Busca el ejecutable del comando en los directorios del PATH */
/* Función principal que busca el ejecutable */

char *find_executable(char *cmd)
{
    char *path;
    
    if ((path = check_command_path(cmd)))
        return (path);
    return (search_in_path(cmd));
}

/* Prepara los argumentos para execve */
char	**prepare_argv(t_cmd *cmd)
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
    (void)msh;

	executable = NULL;
	argv = NULL;
	setup_redirections(input_fd, output_fd);
	if (cmd->arg && find_first_redirect_index(cmd->arg) != -1)
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


