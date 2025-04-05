/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:24:00 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/05 17:24:00 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*Libera la memoria asignada a un nodo de comando y todos sus componentes. Puntero al nodo de comando a liberar*/
void free_command_node(t_cmd *cmd)
{
	int i;
	
	if (!cmd)
		return;
	
	// Liberar el nombre del comando
	if (cmd->cmd)
		free(cmd->cmd);
	
	// Liberar los argumentos
	if (cmd->arg)
	{
		i = 0;
		while (i < cmd->num_arg && cmd->arg[i])
		{
			free(cmd->arg[i]);
			i++;
		}
		free(cmd->arg);
	}
	// Liberar los archivos de entrada/salida
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	
	// Liberar el nodo en sí
	free(cmd);
}

/*Libera toda la lista enlazada de nodos de comando. Puntero al primer nodo de la lista*/
void free_command_list(t_cmd *cmd)
{
	t_cmd *temp;
	
	while (cmd)
	{
		temp = cmd->next;
		free_command_node(cmd);
		cmd = temp;
	}
}

/* Libera todos los recursos asignados al shell. Estructura principal del shell*/
void cleanup_shell(t_msh *shell)
{
	int i;
	
	if (!shell)
		return;
	
	// Liberar la lista de comandos
	free_command_list(shell->cmd);
	
	// Liberar el entorno
	if (shell->env)
	{
		i = 0;
		while (i < shell->num_env && shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	
	// Liberar el PATH
	if (shell->path)
		free(shell->path);
	
	// Liberar la estructura principal
	free(shell);
}