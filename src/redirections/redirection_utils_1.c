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

static char	**extract_filename_from_arg(char **args, int index, char opChar, int offset)
{
	int	i;

	i = 0;
	// Si el argumento empieza con "2>", salta el '2'
	if (args[index][i] == '2' && args[index][i + 1] == '>')
		i++;
	// Salta todos los caracteres del operador (por ejemplo, '>' o '<')
	while (args[index][i] == opChar)
		i++;
	// Si después del operador no queda nada, no se hace modificación.
	if (!args[index][i])
		return (args);
	// Si no se ha saltado ningún operador, utiliza keep_argument para ajustar el argumento.
	if (i == 0)
		return (keep_argument(args, index, opChar));
	// Inserta la parte anterior al operador en el arreglo.
	args = insert_argument_at_index(args, ft_substr(args[index], 0, i), index + offset++);
	// Inserta la parte correspondiente al operador.
	args = insert_argument_at_index(args, ft_substr(args[index], i,
				find_next_redirect_operator_index(i, args[index]) - i),
				index + offset++);
	// Actualiza 'i' para avanzar hasta el final de la parte del operador.
	i = find_next_redirect_operator_index(i, args[index]);
	// Si queda parte adicional (el sufijo), la inserta en el arreglo.
	if (args[index][i])
		args = insert_argument_at_index(args, ft_substr(args[index], i,
							ft_strlen(args[index]) - i),
							index + offset++);
	// Elimina el elemento original, ya que fue dividido en varias partes.
	args = remove_argument_at_index(args, index);
	return (args);
}


void	handle_output_redirection(int redirType, t_cmd *command, char *filename)
{
	int	fd;
	int	openFlags;

	// Selecciona la bandera de apertura según el tipo de redirección:
	// - Para redirecciones simples y stderr (sobrescritura): truncate (O_TRUNC)
	// - Para redirecciones con append: O_APPEND
	if (redirType == 1 || redirType == 5)
		openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	else
		openFlags = O_CREAT | O_WRONLY | O_APPEND;

	fd = open(filename, openFlags, 0644);
	if (fd == -1)
		exit_error("Error al abrir el archivo de salida", 44);

	// Redirecciona según el descriptor:
	// - Los tipos 5 y 6 redirigen el error estándar
	// - Los demás redirigen el estándar de salida
	if (redirType == 5 || redirType == 6)
	{
		if (dup2(fd, command->pipes->out_error) == -1)
			exit_error("Error al redirigir stderr", 45);
	}
	else
	{
		if (dup2(fd, command->pipes->out) == -1)
			exit_error("Error al redirigir stdout", 45);
	}

	free(filename);
	close(fd);
}
void	handle_here_document(t_cmd *command, char *delimiter)
{
	char	*inputLine;
	int		pipeFd[2];

	// Se crea un pipe para almacenar las líneas del here-document.
	if (pipe(pipeFd) == -1)
		exit_error("Error al crear pipe para here-document", 47);

	// Se muestra un prompt para indicar al usuario que ingrese datos.
	if (write(1, "> ", 2) == -1)
		exit_error("Error de escritura en prompt", 48);

	// Se lee la primera línea de entrada desde el descriptor asociado.
	inputLine = get_next_line(command->pipes->in);
	while (inputLine && ft_strncmp(inputLine, delimiter, ft_strlen(delimiter)) != 0)
	{
		// Se muestra el prompt de nuevo para cada línea ingresada.
		if (write(1, "> ", 2) == -1)
			exit_error("Error de escritura en prompt", 48);
		
		// Se escribe la línea en el extremo de escritura del pipe.
		if (write(pipeFd[1], inputLine, ft_strlen(inputLine)) == -1)
			exit_error("Error al escribir en pipe", 30);
		
		free(inputLine);
		inputLine = get_next_line(command->pipes->in);
	}
	// Liberamos la última línea (que coincide con el delimitador o es NULL)
	free(inputLine);
	// Liberamos la memoria asignada al delimitador.
	free(delimiter);

	// Se redirecciona la entrada del comando al extremo de lectura del pipe.
	if (dup2(pipeFd[0], command->pipes->in) == -1)
		exit_error("Error al redireccionar la entrada del here-document", 48);
	
	close(pipeFd[0]);
	close(pipeFd[1]);
}

int	redirect_input_from_file(t_cmd *command, char *fileName, int argIndex)
{
	int	fileDescriptor;
	int	pipeDescriptors[2];

	// Se abre el archivo en modo de solo lectura.
	fileDescriptor = open(fileName, O_RDONLY);
	if (fileDescriptor == -1)
	{
		// Se informa del error y se configura un pipe de respaldo para la salida.
		put_error("bash", fileName, "No such file or directory");
		if (pipe(pipeDescriptors) == -1)
			exit_error("Error al crear pipe para redirección de entrada", 47);
		if (dup2(pipeDescriptors[1], command->pipes->out) == -1)
			exit_error("Error al redirigir salida en caso de fallo en la entrada", 50);
		if (close(pipeDescriptors[0]) == -1 || close(pipeDescriptors[1]) == -1)
			exit_error("Error al cerrar pipe para redirección", 51);
		// Se eliminan del arreglo de argumentos aquellos elementos ya procesados.
		while (command->arg[argIndex])
			command->arg = remove_argument_at_index(command->arg, argIndex);
		free(fileName);
		return (1);
	}
	// Si se abre correctamente, se redirige la entrada estándar al descriptor del archivo.
	else if (dup2(fileDescriptor, command->pipes->in) == -1)
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