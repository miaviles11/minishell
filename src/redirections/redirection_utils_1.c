/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/20 16:46:17 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_output_redirection(int redirType, t_cmd *command, char *filename)
{
	int	fd;
	int	openFlags;

	/* Para redirecciones simples o de error (sobrescritura), usamos O_TRUNC;
	   para append, O_APPEND */
	if (redirType == 1 || redirType == 5)
		openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	else
		openFlags = O_CREAT | O_WRONLY | O_APPEND;
	fd = open(filename, openFlags, 0644);
	if (fd == -1)
		exit_error("Error al abrir el archivo de salida", 44);
	/* Si redirige error (tipos 5 o 6), duplica sobre STDERR_FILENO; 
	   en otro caso, sobre STDOUT_FILENO */
	if (redirType == 5 || redirType == 6)
	{
		if (dup2(fd, STDERR_FILENO) == -1)
			exit_error("Error al redirigir stderr", 45);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			exit_error("Error al redirigir stdout", 45);
	}
	free(filename);
	close(fd);
}

void	handle_here_document(t_cmd *command, char *delimiter)
{
	char	*inputLine;
	int		pipeFd[2];

	/* Crea un pipe para almacenar las líneas del here-document */
	if (pipe(pipeFd) == -1)
		exit_error("Error al crear pipe para here-document", 47);
	if (write(1, "> ", 2) == -1)
		exit_error("Error de escritura en prompt", 48);
	/* Lee desde STDIN_FILENO (la entrada estándar) */
	inputLine = get_next_line(STDIN_FILENO);
	while (inputLine && ft_strncmp(inputLine, delimiter, ft_strlen(delimiter)) != 0)
	{
		if (write(1, "> ", 2) == -1)
			exit_error("Error de escritura en prompt", 48);
		if (write(pipeFd[1], inputLine, ft_strlen(inputLine)) == -1)
			exit_error("Error al escribir en pipe", 30);
		free(inputLine);
		inputLine = get_next_line(STDIN_FILENO);
	}
	free(inputLine);
	free(delimiter);
	/* Duplica el extremo de lectura del pipe a STDIN_FILENO */
	if (dup2(pipeFd[0], STDIN_FILENO) == -1)
		exit_error("Error al redirigir la entrada del here-document", 48);
	close(pipeFd[0]);
	close(pipeFd[1]);
}


int	redirect_input_from_file(t_cmd *command, char *fileName, int argIndex)
{
	int	fileDescriptor;
	int	pipeDescriptors[2];

	/* Intenta abrir el archivo en modo lectura */
	fileDescriptor = open(fileName, O_RDONLY);
	if (fileDescriptor == -1)
	{
		put_error("bash", fileName, "No such file or directory");
		/* En caso de error, crea un pipe de respaldo y redirige la salida estándar */
		if (pipe(pipeDescriptors) == -1)
			exit_error("Error al crear pipe para redirección de entrada", 47);
		if (dup2(pipeDescriptors[1], STDOUT_FILENO) == -1)
			exit_error("Error al redirigir salida en caso de fallo en la entrada", 50);
		if (close(pipeDescriptors[0]) == -1 || close(pipeDescriptors[1]) == -1)
			exit_error("Error al cerrar pipe para redirección", 51);
		/* Elimina del arreglo los argumentos relacionados con esta redirección */
		while (command->arg[argIndex])
			command->arg = remove_argument_at_index(command->arg, argIndex);
		free(fileName);
		return (1);
	}
	/* En caso de éxito, redirige STDIN_FILENO */
	else if (dup2(fileDescriptor, STDIN_FILENO) == -1)
		exit_error("Error al redirigir entrada", 45);
	close(fileDescriptor);
	free(fileName);
	return (0);
}

/*
 * remove_argument_at_index:
 *   Elimina el argumento en la posición 'removalIndex' del arreglo 'argumentList'.
 *   Se libera la memoria del string eliminado y se crea un nuevo arreglo sin dicho elemento.
 *   Devuelve el nuevo arreglo de argumentos (NULL-terminado).
 */
char    **remove_argument_at_index(char **argumentList, int removalIndex)
{
    int     totalArgs;
    int     i, newIndex;
    char    **newArgumentList;

    if (argumentList == NULL)
        return NULL;

    // Contar la cantidad de argumentos actuales.
    totalArgs = 0;
    while (argumentList[totalArgs])
        totalArgs++;

    // Si el índice a remover está fuera de rango, devuelve el arreglo original.
    if (removalIndex < 0 || removalIndex >= totalArgs)
        return argumentList;

    // Libera el string en la posición a eliminar.
    free(argumentList[removalIndex]);

    // Se aloca memoria para el nuevo arreglo de argumentos:
    // Habrá totalArgs - 1 elementos más el NULL final.
    newArgumentList = malloc(sizeof(char *) * totalArgs);
    if (!newArgumentList)
        exit_error("Error de asignación de memoria al remover argumento", 1);

    // Copiar todos los elementos, excepto el que se elimina.
    newIndex = 0;
    for (i = 0; i < totalArgs; i++)
    {
        if (i == removalIndex)
            continue;
        newArgumentList[newIndex] = argumentList[i];
        newIndex++;
    }
    newArgumentList[newIndex] = NULL;

    // Liberar el arreglo original (los strings se transfieren al nuevo arreglo).
    free(argumentList);
    return newArgumentList;
}