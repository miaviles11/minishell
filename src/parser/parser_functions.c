/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_and_split_input(t_msh *shell, char *inputLine, char ***segments)
{
	/* 1. Verifica que las comillas estén balanceadas. */
	shell->quote = check_quotes_balance(inputLine, shell);
	if (shell->quote == 0)
	{
		return (1);
	}
	/* 2. Cuenta el número de pipes en la línea. */
	shell->pipe = count_pipes(inputLine, shell);
    if (shell->pipe == -1)
	{
        return (1);
	}
	/* 4. Divide la línea en segmentos utilizando el pipe ('|') como delimitador. */
	*segments = split_pipes(inputLine, shell);
    if (!(*segments))
	{
        return (1);
	}
	return (0);
}

t_cmd	*create_command_node(t_msh *shell, char *segment)
{
	int		cmd_idx;
	t_cmd	*node;

	node = ft_calloc(1, sizeof(t_cmd));
	if (!node)
		exit_error("Error malloc", 9);

	/* 1) Extraer todos los tokens (incluye operadores y sus operandos) */
	node->num_arg = count_arguments_parser(segment);
	if (node->num_arg > 0)
		node->arg = extract_arguments(shell, segment, node);
	else
		node->arg = NULL;

	/* 2) Encontrar el índice del comando (primer token que NO es operador) */
	cmd_idx = 0;
	while (node->arg
		&& node->arg[cmd_idx]
		&& get_redirect_type(node->arg[cmd_idx]) > 0)
		cmd_idx += 2;

	/* 3) Asignar el comando y eliminarlo del array de args */
	if (node->arg && node->arg[cmd_idx])
	{
		node->cmd = ft_strdup(node->arg[cmd_idx]);
		node->arg = remove_argument_at_index(node->arg, cmd_idx);
		node->num_arg--;
	}
	else
	{
		node->cmd = ft_strdup("");
	}

	/* 4) Enlazar entorno y siguiente */
	node->env  = shell->env;
	node->next = NULL;
	return (node);
}

/*
** perform_expansion:
**   Recorre el nombre del comando y sus argumentos para expandir variables de entorno
**   y el directorio home. Es decir, sustituye:
**     - Variables indicadas con '$' por su valor (utilizando substitute_variables)
**     - La expansión de '~' o similar por el directorio HOME (utilizando expand_home_directory)
**
** Parámetros:
**   command : Puntero doble al nodo de comando (t_cmd) a procesar.
*/
void	perform_expansion(t_msh *msh, t_cmd **command)
{
	int		i;
	char	*var_reminder;
	char	*tmp;

	var_reminder = NULL;

	/* 1) Variables y ~ en el nombre del comando */
	if (has_variable((*command)->cmd))
		(*command)->cmd = substitute_variables(
				msh, *command, (*command)->cmd, &var_reminder);
	if (needs_home_expansion((*command)->cmd))
		(*command)->cmd = expand_home_directory((*command)->cmd);
	/* 2) Elimina comillas del nombre del comando */
	tmp = str_noquotes((*command)->cmd);
	free((*command)->cmd);
	(*command)->cmd = tmp;

	/* Si no hay args, terminamos */
	if (!(*command)->arg)
		return ;

	/* 3) Variables y ~ en cada argumento, luego quitar comillas */
	i = 0;
	while ((*command)->arg[i])
	{
		if (has_variable((*command)->arg[i]))
			(*command)->arg[i] = substitute_variables(
					msh, *command, (*command)->arg[i], &var_reminder);
		if (needs_home_expansion((*command)->arg[i]))
			(*command)->arg[i] = expand_home_directory(
					(*command)->arg[i]);
		/* quitar comillas */
		tmp = str_noquotes((*command)->arg[i]);
		free((*command)->arg[i]);
		(*command)->arg[i] = tmp;
		i++;
	}
}
/*
** get_last_command_node:
**   Recorre la lista de comandos a partir del nodo apuntado por *cmd
**   y retorna el último nodo de la lista.
*/
t_cmd	*get_last_command_node(t_cmd **cmd)
{
	t_cmd	*current;

	current = *cmd;
	while (current && current->next)
		current = current->next;
	return (current);
}
