/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:10:52 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 12:52:13 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Función auxiliar para actualizar o añadir una variable de entorno
void	update_env(t_msh *msh, const char *arg)
{
	char *key;
	char *value;
	char *equal_pos;
	
	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos) // Si no hay '=', solo se exporta la variable sin valor
		return ;

	key = ft_substr(arg, 0, equal_pos - arg);
	value = ft_strdup(equal_pos + 1);

	if (!key || !value)
	{
		free(key);
		free(value);
		return ;
	}
	set_env_var(msh, key, value);
	free(key);
	free(value);
}

int minishell_export(t_msh *msh, char **argv)
{
	int i;
	
	i = 1;
	if (!argv[1])
	{
		minishell_env(msh);
		return (0);
	}
	while (argv[i])
	{
		if (is_valid_identifier(argv[i]))
		{
			update_env(msh, argv[i]);
		}
		else
		{
			ft_printf("export: `%s': not a valid identifier\n", argv[i]);
		}
		i++;
	}
	return (0);
}