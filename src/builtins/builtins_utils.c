/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:45:01 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 18:51:58 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ************************************************** */
/*            Manejo de variables de entorno          */
/* ************************************************** */

int	update_existing_env_var(t_msh *msh, const char *name, const char *value)
{
	int		i;
	char	*new_var;

	i = 0;
	/* 1) Concatenar name + "=" */
	new_var = ft_strjoin(name, "=");
	/* 2) Concatenar el paso anterior con 'value' */
	new_var = ft_strjoin(new_var, value);
	while (i < msh->num_env)
	{
		/* Chequear si la variable ya existe en msh->env */
		if (ft_strncmp(msh->env[i], name, ft_strlen(name)) == 0
			&& msh->env[i][ft_strlen(name)] == '=')
		{
			/* Si existe, liberar la vieja y sustituirla */
			free(msh->env[i]);
			msh->env[i] = new_var;
			return (0);
		}
		i++;
	}
	/* Si no está, liberamos new_var y devolvemos 1 */
	free(new_var);
	return (1);
}

int	add_new_env_var(t_msh *msh, const char *name, const char *value)
{
	int		i;
	char	*new_var;
	char	**new_env;

	/* name + "=" */
	new_var = ft_strjoin(name, "=");
	/* concatenamos con 'value' */
	new_var = ft_strjoin(new_var, value);

	/* Reservamos espacio para un array con una variable más */
	new_env = malloc(sizeof(char *) * (msh->num_env + 1));
	if (!new_env)
		return (1);

	i = 0;
	/* Copiamos el contenido anterior */
	while (i < msh->num_env)
	{
		new_env[i] = msh->env[i];
		i++;
	}
	/* Añadimos la nueva variable */
	new_env[msh->num_env] = new_var;

	/* Liberamos el array antiguo y actualizamos punteros */
	free(msh->env);
	msh->env = new_env;
	msh->num_env++;
	return (0);
}

int	set_env_var(t_msh *msh, const char *name, const char *value)
{
	/*
	 * Primero intentamos actualizarla (si existe);
	 * Si update_existing_env_var() devuelve 0 es que la encontró y cambió,
	 * y ahí terminamos. Si devolvió 1, es que no existía, entonces la añadimos.
	 */
	if (update_existing_env_var(msh, name, value) == 0)
		return (0);
	return add_new_env_var(msh, name, value);
}

int	is_valid_identifier(const char *var)
{
	int i;

	i = 1;
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (0);
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}