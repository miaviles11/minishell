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

int	exec_builtin(t_msh *msh, char **argv)
{
	if (strcmp(argv[0], "cd") == 0)
		return (minishell_cd(msh, argv));
	if (strcmp(argv[0], "pwd") == 0)
		return (minishell_pwd(msh));
	if (strcmp(argv[0], "echo") == 0)
		return (minishell_echo(msh));
	if (strcmp(argv[0], "exit") == 0)
		return (minishell_exit(msh));
	if (strcmp(argv[0], "env") == 0)
		return (minishell_env(msh));
	if (strcmp(argv[0], "export") == 0)
		return (minishell_export(msh, argv));
	if (strcmp(argv[0], "unset") == 0)
		return (minishell_unset(msh, argv));
	return (0);
}

int	is_builtin(char *cmd)
{
	// Lista de builtins
	if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "echo") == 0 ||
		strcmp(cmd, "exit") == 0 || strcmp(cmd, "env") == 0 || strcmp(cmd, "export") == 0 ||
		strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_msh *msh, t_cmd *cmd)
{
	if (is_builtin(cmd->cmd))
	{
		exec_builtin(msh,cmd->arg);
	}
}