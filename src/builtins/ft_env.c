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
	int		i;
	char	*equal_sign;

	i = 0;
	while (msh->env[i] != NULL)
	{
		equal_sign = ft_strchr(msh->env[i], '=');
		if (equal_sign != NULL && *(equal_sign + 1) != '\0') // Verifica que haya algo después del '='
			ft_printf("%s\n", msh->env[i]);
		i++;
	}
	return (0);
}