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
	/* 1. Verifica que las comillas estén balanceadas.
	   check_quotes_balance retorna un valor distinto de 0 si están balanceadas.
	*/
	shell->quote = check_quotes_balance(inputLine, shell);
	if (shell->quote == 0)
	{
		free(inputLine);
		return (1);
	}
	/* 2. Cuenta el número de pipes en la línea.
	   count_pipes retorna -1 si hay error de sintaxis (por ejemplo, pipe mal colocado).
	*/
	shell->pipe = count_pipes(inputLine);
	if (shell->pipe == -1)
	{
		free(inputLine);
		return (1);
	}
	/* 3. Divide la línea en segmentos utilizando el pipe ('|') como delimitador.
	   split_pipes devuelve un arreglo de cadenas (cada una es un segmento).
	*/
	*segments = split_pipes(shell, inputLine);
	if (!(*segments))
	{
		free(inputLine);
		return (1);
	}
	/* Si todo es correcto, retorna 0 indicando que no hubo errores */
	return (0);
}
static t_cmd	*create_command_node(t_msh *shell, char *segment)
{
	t_cmd	*node;

	node = ft_calloc(1, sizeof(t_cmd));
	if (!node)
		exit_error("Error malloc", 9);
	// Extrae el nombre del comando desde el segmento (a partir de shell->total_chars)
	node->cmd = extract_command(shell, segment + shell->total_chars);
	// Cuenta el número de argumentos en el segmento
	node->num_arg = count_arguments(segment + shell->total_chars);
	node->next = NULL;
	// Asigna las referencias globales (entorno y banderas)
	node->flags = shell->flags;
	node->env = shell->env;
	// Si no se encontraron argumentos, retorna el nodo sin extraer 'arg'
	if (node->num_arg == 0)
		return (node);
	// Si hay argumentos, se extraen y se almacenan en 'arg'
	node->arg = extract_arguments(shell, segment + shell->total_chars, node);
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
	int	i;

	// Procesa el nombre del comando.
	if (has_variable((*command)->cmd))
		(*command)->cmd = substitute_variables(msh, *command, (*command)->cmd, NULL);
	if (needs_home_expansion((*command)->cmd))
		(*command)->cmd = expand_home_directory((*command)->cmd);
	// Si no hay argumentos, terminamos.
	if (!(*command)->arg)
		return;
	i = 0;
	// Procesa cada argumento del comando.
	while ((*command)->arg[i])
	{
		if (has_variable((*command)->arg[i]))
			(*command)->arg[i] = substitute_variables(msh, *command, (*command)->arg[i], NULL);
		if (needs_home_expansion((*command)->arg[i]))
			(*command)->arg[i] = expand_home_directory((*command)->arg[i]);
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
