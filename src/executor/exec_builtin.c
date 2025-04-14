/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:08:26 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 19:38:13 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute_builtin_with_redirection(t_msh *msh, t_cmd *cmd, int output_fd)
{
    // Redirigir la salida estándar al extremo de escritura del pipe
    if (output_fd != STDOUT_FILENO)
    {
        if (dup2(output_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(output_fd);
            exit(1);
        }
        close(output_fd);
    }
    // Ejecutar el builtin correspondiente
    if (ft_strncmp(cmd->cmd, "env", 4) == 0)
        minishell_env(msh);
    else if (ft_strncmp(cmd->cmd, "echo", 5) == 0)
        minishell_echo(msh);
    else if (ft_strncmp(cmd->cmd, "cd", 3) == 0)
        minishell_cd(msh, cmd->arg); // Pasar cmd->arg como segundo argumento
    else if (ft_strncmp(cmd->cmd, "pwd", 4) == 0)
        minishell_pwd(msh);
    else if (ft_strncmp(cmd->cmd, "export", 7) == 0)
        minishell_export(msh, cmd->arg); // Pasar cmd->arg como segundo argumento
    else if (ft_strncmp(cmd->cmd, "unset", 6) == 0)
        minishell_unset(msh, cmd->arg); // Pasar cmd->arg como segundo argumento
    else if (ft_strncmp(cmd->cmd, "exit", 5) == 0)
        minishell_exit(msh);
    else
    {
        ft_printf("Command not found: %s\n", cmd->cmd);
        exit(127);
    }
    // Salir del proceso hijo después de ejecutar el builtin
    exit(0);
}

int	exec_builtin(t_msh *msh, char **argv)
{
	// Verificar que los punteros no sean NULL
	if (!msh || !argv || !argv[0])
		return (1);
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (minishell_cd(msh, argv));
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (minishell_pwd(msh));
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (minishell_echo(msh));
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (minishell_exit(msh));
	if (ft_strncmp(argv[0], "env", 4) == 0)
		return (minishell_env(msh));
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (minishell_export(msh, argv));
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (minishell_unset(msh, argv));
	return (0);
}

int	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (0);
	// Lista de builtins
	if (ft_strncmp(cmd_name, "cd", 3) == 0 || ft_strncmp(cmd_name, "pwd", 4) == 0 ||
		ft_strncmp(cmd_name, "echo", 5) == 0 || ft_strncmp(cmd_name, "exit", 5) == 0 ||
		ft_strncmp(cmd_name, "env", 4) == 0 || ft_strncmp(cmd_name, "export", 7) == 0 ||
		ft_strncmp(cmd_name, "unset", 6) == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_msh *msh, t_cmd *cmd)
{
	char	**argv;
	int		i;
	int		j;

	i = 0;
	if (!msh || !cmd || !cmd->cmd)
		return ;
	if (cmd->arg)
	{
		while (cmd->arg[i++])
			i++;
	}
	argv = (char **)malloc(sizeof(char *) * (i + 2));
	if (!argv)
	{
		free(argv);
		return ;
	}
	argv[0] = cmd->cmd;
	j = -1;
	while (++j < i)
		argv[j + 1] = cmd->arg[j];
	argv[i + 1] = NULL;
	msh->error_value = exec_builtin(msh, argv);
	free(argv);
}
