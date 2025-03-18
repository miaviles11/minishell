/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:23:48 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 19:37:54 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

char	*find_executable(char *cmd);
void	handle_redirection(t_cmd *cmd);
void	execute_command(t_cmd *cmd);
void	execute_commands(t_msh *msh);

int		exec_builtin(t_msh *msh, char **argv);
int 	is_builtin(char *cmd);
void	execute_builtin(t_msh *msh, t_cmd *cmd);

#endif