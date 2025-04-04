/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:48:18 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 18:52:23 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	minishell_env(t_msh *msh)
{
    int	i;

    i = 0;
    while (msh->env[i] != NULL)
    {
        ft_printf("%s\n", msh->env[i]);
        i++;
    }
    return (0);
}