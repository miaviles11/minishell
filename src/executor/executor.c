/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:39:20 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/02 11:22:43 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Busca el ejecutable del comando en los directorios del PATH
char	*find_executable(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char 	*temp;
	int		i;

	// Obtiene la variable de entorno PATH
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	// Divide la variable PATH en un array de directorios
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	// Recorre cada directorio en PATH para buscar el ejecutable
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

// Maneja la redirección de entrada y salida para un comando
void	handle_redirection(t_cmd *cmd)
{
	int	fd;

	// Redirige la entrada si se especifica un archivo de entrada
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
		{
			perror("open input_file");
			_exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	// Redirige la salida si se especifica un archivo de salida
	if (cmd->output_file)
	{
		fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open output_file");
			_exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

// Ejecuta un comando específico
void	execute_command(t_msh *msh, t_cmd *cmd)
{
	char	*executable;
	pid_t	pid;

	// Verifica si el comando es válido
	if (!cmd || !cmd->cmd)
		return ;
	// Ejecuta el comando si es un comando interno
	if (is_builtin(cmd->cmd))
		return (execute_builtin(msh, cmd));
	// Busca el ejecutable del comando
	executable = find_executable(cmd->cmd);
	if (!executable)
	{
		ft_printf("Command not found: %s\n", cmd->cmd);
		return ;
	}
	// Crea un proceso hijo para ejecutar el comando
	pid = fork();
	if (pid == 0)
	{
		handle_redirection(cmd); // Maneja la redirección en el proceso hijo
		execve(executable, cmd->arg, NULL); // Ejecuta el comando
		perror("execve");
		_exit(1);
	}
	else if (pid > 0 && !cmd->background)
		waitpid(pid, NULL, 0); // Espera a que el proceso hijo termine si no es en segundo plano
	else if (pid < 0)
		perror("fork");
	free(executable); // Libera la memoria del ejecutable
}

// Ejecuta una lista de comandos
void	execute_commands(t_msh *msh)
{
	t_cmd	*current;

	current = msh->cmd;
	while (current)
	{
		execute_command(msh, current); // Ejecuta cada comando en la lista
		current = current->next;
	}
}