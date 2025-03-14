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

int	minishell_echo(char **argv)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		no_newline = 1;
		i = 2;
	}
	while (argv[i] != NULL)
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!no_newline)
		ft_printf("\n");
	return (0);
}
