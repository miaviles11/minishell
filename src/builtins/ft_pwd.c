/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:28:09 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/13 17:16:49 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_pwd(t_msh *msh)
{
    char	cwd[1024];

    (void)msh;
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("pwd");
        return (1);
    }
    ft_printf("%s\n", cwd);
    return (0);
}