/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:51:04 by carlsanc          #+#    #+#             */
/*   Updated: 2025/05/05 16:51:04 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	redir_only_child(t_cmd *cmd)
{
	pid_t	pid;
    ssize_t	n;
	int		status;
    char	buf[4096];

	pid = fork();
	if (pid == -1)
		return (perror("fork"));
	if (pid == 0)
	{
		if (cmd->arg && find_first_redirect_index(cmd->arg) != -1)
			process_redirections(cmd);
		if (!isatty(STDIN_FILENO))
		{
			while ((n = read(STDIN_FILENO, buf, sizeof buf)) > 0)
				write(STDOUT_FILENO, buf, n);
		}
		_exit(0);
	}
	waitpid(pid, &status, 0);
}

