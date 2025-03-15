/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 15:08:26 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/15 15:08:26 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_builtin(char **argv)
{
	if (strcmp(argv[0], "cd") == 0)
		return minishell_cd(argv);
	if (strcmp(argv[0], "pwd") == 0)
		return minishell_pwd(argv);
	if (strcmp(argv[0], "echo") == 0)
		return minishell_echo(argv);
	if (strcmp(argv[0], "exit") == 0)
		return minishell_exit(argv);
	if (strcmp(argv[0], "env") == 0)
		return minishell_env(argv);
	if (strcmp(argv[0], "export") == 0)
		return minishell_export(argv);
	if (strcmp(argv[0], "unset") == 0)
		return minishell_unset(argv);
}