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

/**
 * Ejecuta un comando individual (sin pipes)
 */
static void	execute_single_command(t_msh *msh, t_cmd *cmd)
{
    char	*executable;
    pid_t	pid;
    int		status;
    char	**argv;
    int		i;
    int		j;

    if (!cmd || !cmd->cmd)
        return;
    executable = find_executable(cmd->cmd);
    if (!executable)
    {
        ft_printf("Command not found: %s\n", cmd->cmd);
        msh->error_value = 127;
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(executable);
        msh->error_value = 1;
        return;
    }
    if (pid == 0)
    {
        /* Procesa redirecciones basadas en los tokens (cmd->arg) */
        if (msh->redic && cmd->arg && find_first_redirect_index(cmd->arg) != -1)
            process_redirections(cmd);
        if (cmd->arg)
        {
            i = 0;
            while (cmd->arg[i])
                i++;
        }
        else
            i = 0;
        argv = (char **)malloc(sizeof(char *) * (i + 2));
        if (!argv)
        {
            free(argv);
            _exit(1);
        }
        argv[0] = cmd->cmd;
        j = -1;
        while (++j < i)
            argv[j + 1] = cmd->arg[j];
        argv[i + 1] = NULL;
        execve(executable, argv, cmd->env);
        perror("execve");
        free(executable);
        free(argv);
        _exit(1);
    }
    else if (pid > 0 && !cmd->background)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            msh->error_value = WEXITSTATUS(status);
    }
    free(executable);
}

/**
 * Procesa un comando con su pipe de salida
 */
static void process_cmd_with_pipe(t_msh *msh, t_cmd *cmd, int prev_pipe, int *pipe_fd)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        if (prev_pipe != STDIN_FILENO)
            close(prev_pipe);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return;
    }
    if (pid == 0)
    {
        if (is_builtin(cmd->cmd))
        {
            close(pipe_fd[0]); // Cerrar el extremo de lectura del pipe
            execute_builtin_with_redirection(msh, cmd, pipe_fd[1]);
            exit(0);
        }
        close(pipe_fd[0]);
        child_process(msh, cmd, prev_pipe, pipe_fd[1]);
    }
    if (prev_pipe != STDIN_FILENO)
        close(prev_pipe);
    close(pipe_fd[1]);
}


/**
 * Procesa el último comando en un pipeline
 */
static void process_last_cmd(t_msh *msh, t_cmd *cmd, int prev_pipe)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        if (prev_pipe != STDIN_FILENO)
            close(prev_pipe);
        return;
    }
    if (pid == 0)
    {
        if (is_builtin(cmd->cmd))
        {
            execute_builtin_with_redirection(msh, cmd, STDOUT_FILENO);
            exit(0);
        }
        child_process(msh, cmd, prev_pipe, STDOUT_FILENO);
    }
    if (prev_pipe != STDIN_FILENO)
        close(prev_pipe);
    if (!cmd->background)
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            msh->error_value = WEXITSTATUS(status);
    }
}

/**
 * Espera a que terminen todos los procesos hijos pendientes
 */
static void	wait_for_children(void)
{
	pid_t	wpid;
	int		status;

	while ((wpid = wait(&status)) > 0)
		;
}

/**
 * Ejecuta una lista de comandos con pipes
 */
void execute_commands(t_msh *msh)
{
    t_cmd *current;
    t_cmd *next;
    int pipe_fd[2];
    int prev_pipe;

    if (!msh->cmd)
        return;
    if (!msh->cmd->next && !msh->pipe)
    {
        if (is_builtin(msh->cmd->cmd))
            execute_builtin(msh, msh->cmd);
        else
            execute_single_command(msh, msh->cmd);
        return;
    }
    current = msh->cmd;
    prev_pipe = STDIN_FILENO;
    while (current)
    {
        next = current->next;
        if (next)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return;
            }
            process_cmd_with_pipe(msh, current, prev_pipe, pipe_fd);
            prev_pipe = pipe_fd[0];
        }
        else
            process_last_cmd(msh, current, prev_pipe);
        current = next;
    }
    if (!msh->cmd->background)
        wait_for_children();
}

/**
 * Maneja la redirección de entrada y salida para un comando
 
void	handle_redirection(t_cmd *cmd)
{
	int	fd;

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
}*/