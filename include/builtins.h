/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:46:08 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 17:28:57 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		update_existing_env_var(t_msh *msh, const char *name, const char *value);
int		add_new_env_var(t_msh *msh, const char *name, const char *value);
int		set_env_var(t_msh *msh, const char *name, const char *value);
int		is_valid_identifier(const char *var);

int		minishell_cd(t_msh *msh, char **argv);

void	print_arguments(t_cmd *cmd, int i);
int		handle_no_newline(t_cmd *cmd, int *no_newline);
int		minishell_echo(t_msh *msh);

int		minishell_pwd(t_msh *msh);

int		is_numeric(const char *str);
int		minishell_exit(t_msh *msh);

int		minishell_env(t_msh *msh);

int		minishell_export(t_msh *msh, char **argv);
void	update_env(t_msh *msh, const char *arg);

int		minishell_unset(t_msh *msh, char **args);
void	remove_env_var(t_msh *msh, const char *key);

#endif