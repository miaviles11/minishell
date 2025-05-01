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

static void exec_child(t_msh *msh, t_cmd *cmd, char *executable)
{
    setup_child_signals();

    if (cmd->arg && find_first_redirect_index(cmd->arg) != -1)
        process_redirections(cmd);
    perform_expansion(msh, &cmd);

    char **argv = prepare_argv(cmd);
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

static void consume_here_docs(char **args)
{
    int  i = 0;
    char *line;
    char *delimiter;

    while (args && args[i])
    {
        if (get_redirect_type(args[i]) == 4)
        {
            delimiter = str_noquotes(args[i + 1]);
            while (1)
            {
                write(STDERR_FILENO, "> ", 2);
                line = get_next_line(STDIN_FILENO);
                if (!line)
                    break;
                if (!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
                {
                    free(line);
                    break;
                }
                free(line);
            }
            free(delimiter);
            i += 2;
        }
        else
            i++;
    }
}

static void execute_single_command(t_msh *msh, t_cmd *cmd)
{
    pid_t pid;
    int   status;
    char *executable;

    if (!cmd->cmd || cmd->cmd[0] == '\0')
    {
        consume_here_docs(cmd->arg);
        msh->error_value = 0;
        return;
    }
    perform_expansion(msh, &cmd);
    executable = find_executable(cmd->cmd);
    if (!executable)
    {
        ft_printf("Command not found: %s\n", cmd->cmd);
        msh->error_value = 127;
        return;
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free(executable);
        msh->error_value = 1;
        return;
    }
    if (pid == 0)
    {
        exec_child(msh, cmd, executable);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        msh->error_value = WEXITSTATUS(status);

    free(executable);
}

static void process_cmd_with_pipe(t_msh *msh, t_cmd *cmd, int prev_pipe, int *pipe_fd)
{
    pid_t pid;
    int   status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        if (prev_pipe != STDIN_FILENO)
        {
            dup2(prev_pipe, STDIN_FILENO);
            close(prev_pipe);
        }
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        if (cmd->arg && find_first_redirect_index(cmd->arg) != -1)
            process_redirections(cmd);
        perform_expansion(msh, &cmd);
        if (is_builtin(cmd->cmd))
        {
            execute_builtin_with_redirection(msh, cmd, STDOUT_FILENO);
            _exit(msh->error_value);
        }
        else
        {
            char **argv = prepare_argv(cmd);
            char *exe   = find_executable(cmd->cmd);
            execve(exe, argv, msh->env);
            perror("execve");
            _exit(1);
        }
    }
    if (prev_pipe != STDIN_FILENO)
        close(prev_pipe);
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        msh->error_value = WEXITSTATUS(status);
}

static void process_last_cmd(t_msh *msh, t_cmd *cmd, int prev_pipe)
{
    pid_t pid;
    int   status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        if (prev_pipe != STDIN_FILENO)
        {
            dup2(prev_pipe, STDIN_FILENO);
            close(prev_pipe);
        }

        if (cmd->arg && find_first_redirect_index(cmd->arg) != -1)
            process_redirections(cmd);

        perform_expansion(msh, &cmd);

        if (is_builtin(cmd->cmd))
        {
            execute_builtin_with_redirection(msh, cmd, STDOUT_FILENO);
        }
        else
        {
            char **argv = prepare_argv(cmd);
            char *exe   = find_executable(cmd->cmd);
            execve(exe, argv, msh->env);
            perror("execve");
            _exit(1);
        }
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
