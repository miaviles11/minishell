/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:46:08 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/15 12:46:08 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		update_existing_env_var(t_msh *msh, const char *name, const char *value);
int		add_new_env_var(t_msh *msh, const char *name, const char *value);
int		set_env_var(t_msh *msh, const char *name, const char *value);
int		minishell_cd(char **argv);

int		minishell_pwd(char **argv);

void	print_arguments(char **argv, int i);
int		handle_no_newline(char **argv, int *no_newline);
int		minishell_echo(char **argv);

int		minishell_exit(char **argv);

int		minishell_env(char **argv);

int		minishell_export(char **argv);

int		minishell_unset(char **argv);

#endif