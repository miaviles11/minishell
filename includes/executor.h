/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:23:48 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/02 11:20:44 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <sys/wait.h>

char	*find_executable(char *cmd);
void	handle_redirection(t_cmd *cmd);
void	child_process(t_msh *msh, t_cmd *cmd, int input_fd, int output_fd);
void	execute_commands(t_msh *msh);

int		exec_builtin(t_msh *msh, char **argv);
int		is_builtin(char *cmd_name);
void	execute_builtin(t_msh *msh, t_cmd *cmd);

#endif