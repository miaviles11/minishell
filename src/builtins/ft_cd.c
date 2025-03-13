/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:16:24 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/13 16:26:52 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_cd(char **argv)
{
	char	*path;
	
	path = NULL;
	if (argv[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_printf(stderr, "cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = argv[1];
	if (chdir(path) != 0)
	{
		perror ("cd");
		return (1);
	}
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		setenv("PWD", cwd, 1);
	return (0);
}
