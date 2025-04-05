/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:40:42 by miaviles          #+#    #+#             */
/*   Updated: 2025/04/05 16:40:42 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Inicializa la estructura principal del shell.
 * Asigna memoria para la estructura y sus componentes, y configura
 * el entorno a partir del envp recibido.
 * 
 *  Array de strings con las variables de entorno
 *  Puntero a la estructura t_msh inicializada o NULL en caso de error
 */

t_msh *initialize_shell(char **envp)
{
    t_msh *shell;
    int i;

    // Asignar memoria para la estructura principal
    shell = (t_msh *)malloc(sizeof(t_msh));
    if (!shell)
        return (NULL);

    // Inicializar todos los campos a valores por defecto
    shell->bash_lvl = 0;
    shell->total_chars = 0;
    shell->cmd = NULL;
    shell->path = NULL;
    shell->error_value = 0;
    shell->quote = 0;
    shell->pipe = 0;
    shell->redic = 0;

    // Calcular el número de variables de entorno
    shell->num_env = 0;
    if (envp)
    {
        while (envp[shell->num_env])
            shell->num_env++;
    }

    // Duplicar el entorno
    shell->env = (char **)malloc(sizeof(char *) * (shell->num_env + 1));
    if (!shell->env)
    {
        free(shell);
        return (NULL);
    }

    for (i = 0; i < shell->num_env; i++)
    {
        shell->env[i] = strdup(envp[i]);
        if (!shell->env[i])
        {
            // Liberar memoria asignada en caso de error
            while (--i >= 0)
                free(shell->env[i]);
            free(shell->env);
            free(shell);
            return (NULL);
        }
    }
    shell->env[shell->num_env] = NULL;

    // Extraer el PATH del entorno
    shell->path = get_env_value(shell, "PATH");

    // Incrementar SHLVL (Shell Level)
    update_shell_level(shell);

    return (shell);
}

/**
 * Obtiene el valor de una variable de entorno
 * 
 * @param shell Estructura principal del shell
 * @param var_name Nombre de la variable a buscar
 * @return Valor de la variable o NULL si no existe
 */
char *get_env_value(t_msh *shell, const char *var_name)
{
    int i;
    int len;
    
    if (!shell || !var_name || !shell->env)
        return (NULL);
    
    len = strlen(var_name);
    
    for (i = 0; i < shell->num_env; i++)
    {
        if (strncmp(shell->env[i], var_name, len) == 0 && 
            shell->env[i][len] == '=')
        {
            return (strdup(shell->env[i] + len + 1));
        }
    }
    
    return (NULL);
}

/**
 * Actualiza la variable SHLVL en el entorno
 * Incrementa el nivel del shell cada vez que se inicia uno nuevo
 * 
 * Estructura principal del shell
 */
void update_shell_level(t_msh *shell)
{
    char *shlvl_str;
    int shlvl;
    char *new_shlvl;
    char *new_shlvl_entry;
    
    // Obtener el valor actual de SHLVL
    shlvl_str = get_env_value(shell, "SHLVL");
    
    // Convertir a entero e incrementar
    shlvl = 1;  // Valor por defecto si no existe
    if (shlvl_str)
    {
        shlvl = atoi(shlvl_str);
        free(shlvl_str);
        shlvl++;
    }
    
    // Actualizar bash_lvl en la estructura
    shell->bash_lvl = shlvl;
    
    // Convertir el nuevo valor a string
    new_shlvl = ft_itoa(shlvl);  // Asumiendo que tienes esta función
    if (!new_shlvl)
        return;
    
    // Crear la nueva entrada para el entorno
    new_shlvl_entry = ft_strjoin("SHLVL=", new_shlvl);  // Asumiendo que tienes esta función
    free(new_shlvl);
    
    if (!new_shlvl_entry)
        return;
    
    // Actualizar el entorno
    set_env_value(shell, "SHLVL", new_shlvl_entry + 6);
    free(new_shlvl_entry);
}

/**
 * Establece o actualiza una variable de entorno
 * 
 * param shell Estructura principal del shell
 * param var_name Nombre de la variable
 * param value Nuevo valor
 * return 0 en caso de éxito, -1 en caso de error
 */
int set_env_value(t_msh *shell, const char *var_name, const char *value)
{
    int i;
    int len;
    char *new_entry;
    
    if (!shell || !var_name || !value)
        return (-1);
    len = strlen(var_name);
    // Verificar si la variable ya existe
    for (i = 0; i < shell->num_env; i++)
    {
        if (strncmp(shell->env[i], var_name, len) == 0 && 
            shell->env[i][len] == '=')
        {
            // La variable existe, actualizarla
            free(shell->env[i]);
            new_entry = malloc(len + strlen(value) + 2);
            if (!new_entry)
                return (-1);
            strcpy(new_entry, var_name);
            strcat(new_entry, "=");
            strcat(new_entry, value);
            shell->env[i] = new_entry;
            return (0);
        }
    }

    // La variable no existe, añadirla al final
    // Esto requeriría realocar el array env, lo que podría ser complejo
    // Aquí una implementación básica:
    char **new_env = realloc(shell->env, sizeof(char*) * (shell->num_env + 2));
    if (!new_env)
        return (-1);
    
    shell->env = new_env;
    
    new_entry = malloc(len + strlen(value) + 2);
    if (!new_entry)
        return (-1);
    strcpy(new_entry, var_name);
    strcat(new_entry, "=");
    strcat(new_entry, value);
    shell->env[shell->num_env] = new_entry;
    shell->env[shell->num_env + 1] = NULL;
    shell->num_env++;
    return (0);
}
