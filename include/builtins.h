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

int	minishell_cd(char **argv);
int	minishell_pwd(char **argv);
int	minishell_echo(char **argv);
int	minishell_exit(char **argv);
int	minishell_env(char **argv);
int	minishell_export(char **argv);
int	minishell_unset(char **argv);

#endif