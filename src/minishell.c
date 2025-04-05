/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/20 16:59:54 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_extra_arguments(char **argv)
{
    // Imprimir un mensaje de error indicando que no se aceptan argumentos
    fprintf(stderr, "Error: minishell no acepta argumentos adicionales\n");
    fprintf(stderr, "Uso: %s\n", argv[0]);
    
    // Terminar el programa con código de error
    exit(1);
}

void	run_shell_loop(t_msh *shell)
{
	char	*input_line;

	while (1)
	{
		/* Se muestra el prompt simple y se lee la línea de comando */
		input_line = readline(WHITE_T "minishell-> " RESET_COLOR);
		if (!input_line)
			exit_error("exit", shell->error_value);
		/* Si la línea está vacía o solo contiene espacios, se descarta */
		if (is_line_empty(input_line))
		{
			free(input_line);
			continue ;
		}
		add_history(input_line);
		/* Se parsea la entrada y se verifican las comillas */
		if (parse_input_line(shell, &shell->cmd, input_line)
			&& shell->quote != 0)
		{
			/* Ejecuta el comando de forma individual o en pipeline según corresponda */
			if (is_builtin(shell->cmd->cmd))
				execute_builtin(shell, shell->cmd);
			else
				execute_commands(shell);
		}
	}
}

int    main(int argc, char **argv, char **envp)
{
    t_msh *shell;

    if (argc != 1)
        handle_extra_arguments(argv);
    // Inicializa shell primero, luego asigna error_value
    shell = initialize_shell(envp);
    if (!shell)
        return (1);
    shell->error_value = 0;
    setup_signals();
    run_shell_loop(shell);
    return (0);
}