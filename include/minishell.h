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
# include "builtins.h"
# include "env.h"
# include "executor.h"
# include "parser.h"
# include "redirections.h"
# include "signals.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

int	is_line_empty(char *s);
int is_builtin_no_pipeline(t_cmd *cmd, char *cmd_name);
char	*str_noquotes(char *str);
void	run_shell_loop(t_msh *shell);
int	main(int argc, char **argv, char **envp);

#endif