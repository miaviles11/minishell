/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_shell_loop(t_msh *shell)
{
	char	*input_line;

	while (1)
	{
		/* Se muestra el prompt simple y se lee la línea de comando */
		input_line = readline(WHITE_T "minishell-> " RESET_COLOR);
		if (!input_line)
			exit_error("exit", g_error);
		/* Si la línea está vacía o solo contiene espacios, se descarta */
		if (is_line_empty(input_line))
		{
			free(input_line);
			continue ;
		}
		add_history(input_line);
		/* Se parsea la entrada y se verifican las comillas */
		if (parse_input_line(shell, &shell->cmd, input_line)
			&& shell->flags->quote != 0)
		{
			/* Ejecuta el comando de forma individual o en pipeline según corresponda */
			if (is_builtin_no_pipeline(shell->cmd,
					str_noquotes(shell->cmd->cmd)))
				execute_single_command(shell->cmd,
					str_noquotes(shell->cmd->cmd));
			else
				execute_pipeline(&(shell->cmd), initialize_pipes(), 1);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_msh *shell;

	if (argc != 1)
		handle_extra_arguments(argv);
	g_error = 0;
	shell = initialize_shell(envp);
	setup_signal_handlers(1);
	run_shell_loop(shell);
	return (0);
}