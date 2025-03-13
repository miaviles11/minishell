/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:40:51 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/13 18:44:21 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

int	minishell_exit(char **argv)
{
	if (argv[1]) 
	{
		int status = atoi(argv[1]);
		exit(status);
	}
	else
		exit(0);
	return (0);
}