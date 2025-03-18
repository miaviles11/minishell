/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:10:56 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 19:10:03 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Función auxiliar para eliminar una variable de entorno
void	remove_env_var(t_msh *msh, const char *key)
{
	int i;
	int key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (msh->env[i])
	{
		if (!ft_strncmp(msh->env[i], key, key_len) && msh->env[i][key_len] == '=')
		{
			free(msh->env[i]);
			while (msh->env[i])
			{
				msh->env[i] = msh->env[i + 1];
				i++;
			}
			msh->env[i] = NULL;
			msh->num_env--;
			return ;
		}
		i++;
	}
}

// Implementación de ft_unset
int minishell_unset(t_msh *msh, char **args)
{
	int i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n", args[i]);
			continue ;
		}
		remove_env_var(msh, args[i]);
		i++;
	}
	return (0);
}
