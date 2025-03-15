/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:17:45 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/13 17:46:52 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_no_newline(t_cmd *cmd, int *no_newline)
{
	int	i;

	i = 1;
	*no_newline = 0;
	if (cmd->arg[1] && ft_strcmp(cmd->arg[1], "-n") == 0)
	{
		*no_newline = 1;
		i = 2;
	}
	return (i);
}

void	print_arguments(t_cmd *cmd, int i)
{
	while (cmd->arg[i] != NULL)
	{
		ft_printf("%s", cmd->arg[i]);
		if (cmd->arg[i + 1])
			ft_printf(" ");
		i++;
	}
}

int	minishell_echo(t_msh *msh)
{
	int	i;
	int	no_newline;
	t_cmd *cmd;

	cmd = msh->cmd;
	i = handle_no_newline(cmd, &no_newline);
	print_arguments(cmd, i);
	if (!no_newline)
		ft_printf("\n");
	return (0);
}
