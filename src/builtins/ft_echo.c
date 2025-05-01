/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/03/13 17:17:45 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 19:24:53 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	handle_no_newline(t_cmd *cmd, int *no_newline)
{
	int i = 0;
	int j;
	*no_newline = 0;

	while (cmd->arg[i] && cmd->arg[i][0] == '-')
	{
		j = 1;
		while (cmd->arg[i][j] == 'n')
			j++;
		if (j > 1 && cmd->arg[i][j] == '\0')
		{
			*no_newline = 1;
			i++;
		}
		else
			break ;
	}

	return (i);
}

static void	print_arguments(t_cmd *cmd, int start)
{
	int i = start;
	while (cmd->arg[i])
	{
		ft_printf("%s", cmd->arg[i]);
		if (cmd->arg[i + 1])
			ft_printf(" ");
		i++;
	}
}

int	minishell_echo(t_msh *msh)
{
	t_cmd *cmd = msh->cmd;
	int no_newline;
	int i;

	if (!cmd || !cmd->arg || !cmd->arg[0])
	{
		ft_printf("\n");
		return (0);
	}
	i = handle_no_newline(cmd, &no_newline);
	print_arguments(cmd, i);
	if (!no_newline)
		ft_printf("\n");
	return (0);
}
