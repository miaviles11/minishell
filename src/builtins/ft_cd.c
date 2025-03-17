/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:16:24 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 12:45:31 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_cd(t_msh *msh, char **argv)
{
	char	*path;
	char	cwd[1024];

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
		perror("cd");
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_var(msh, "PWD", cwd);
	return (0);
}
