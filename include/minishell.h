/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:19:16 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/13 17:45:21 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

int	minishell_cd(char **argv);
int	minishell_pwd(char **argv);
int	minishell_echo(char **argv);
int	minishell_exit(char **argv);
int	minishell_env(char **argv);
int	minishell_export(char **argv);
int	minishell_unset(char **argv);

int	is_line_empty(char *s);
void	run_shell_loop(t_msh *shell);
int	main(int argc, char **argv, char **envp);

#endif