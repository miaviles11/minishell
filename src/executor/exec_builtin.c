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
		exec_builtin(msh,cmd->arg);
}