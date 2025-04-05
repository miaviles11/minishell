/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:17:45 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 19:24:53 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_no_newline(t_cmd *cmd, int *no_newline)
{
	int	i;

	i = 1;
	*no_newline = 0;
	if (cmd->arg[1] && ft_strncmp(cmd->arg[1], "-n", 3) == 0)
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

/*int main(int argc, char **argv)
{
	t_msh *msh;

	msh = malloc(sizeof(t_msh));
	msh->cmd = malloc(sizeof(t_cmd));
	ft_printf("[1]: ");
	msh->cmd->arg = (char *[]) {"echo", "Hello", "World", NULL};
	minishell_echo(msh);
	ft_printf("[2]: ");
	msh->cmd->arg = (char *[]) {"echo", "-nnnnnnn", "World", NULL};
	minishell_echo(msh);
	ft_printf("[3]: ");
	msh->cmd->arg = (char *[]) {"echo", "-nnnnnnn 3 -nn", "World", "-nn", NULL};
	minishell_echo(msh);
	ft_printf("[4]: ");
	msh->cmd->arg = (char *[]) {"echo", "- ", NULL};
	minishell_echo(msh);
	return (0);
}*/
