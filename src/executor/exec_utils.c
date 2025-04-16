/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:19:08 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/16 17:20:12 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Espera a que terminen todos los procesos hijos pendientes*/

void	wait_for_children(void)
{
	pid_t	wpid;
	int		status;

	while ((wpid = wait(&status)) > 0)
		;
}