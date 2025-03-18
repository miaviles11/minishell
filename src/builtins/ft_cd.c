/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:16:24 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 19:09:50 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_error(const char *message)
{
    int stdout_fd;
	
	stdout_fd = dup(STDOUT_FILENO);
    dup2(STDERR_FILENO, STDOUT_FILENO);
    ft_printf("%s\n", message);
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdout_fd);
}

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
			print_error("cd: HOME not set");
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
