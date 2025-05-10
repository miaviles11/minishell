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
	(void) command;
	int	openFlags;

	if (redirType == 1 || redirType == 5)
		openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	else
		openFlags = O_CREAT | O_WRONLY | O_APPEND;
	fd = open(filename, openFlags, 0644);
	if (fd == -1)
		exit_error("Error al abrir el archivo de salida", 44);
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
	int		p[2];
	int		tty_fd;
	(void)command;

	if (pipe(p) == -1)
		exit_error("Error al crear pipe para here-document", 47);
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
		exit_error("Error al abrir /dev/tty para here-document", 52);
	while (1)
	{
		if (write(STDERR_FILENO, "> ", 2) == -1)
			exit_error("Error de escritura en prompt", 48);
		inputLine = get_next_line(tty_fd);
		if (!inputLine)
			exit_error("EOF inesperado en here-document", 53);
		if (!ft_strncmp(inputLine, delimiter, ft_strlen(delimiter))
			&& inputLine[ft_strlen(delimiter)] == '\n')
		{
			free(inputLine);
			break;
		}
		if (write(p[1], inputLine, ft_strlen(inputLine)) == -1)
			exit_error("Error al escribir en pipe de here-document", 54);
		free(inputLine);
	}
	close(tty_fd);
	close(p[1]);
	if (dup2(p[0], STDIN_FILENO) == -1)
		exit_error("Error al redirigir STDIN para here-document", 55);
	close(p[0]);
	free(delimiter);
}


int	redirect_input_from_file(t_cmd *command, char *fileName, int argIndex)
{
	int	fileDescriptor;
	int	pipeDescriptors[2];

	fileDescriptor = open(fileName, O_RDONLY);
	if (fileDescriptor == -1)
	{
		put_error("bash", fileName, "No such file or directory");
		if (pipe(pipeDescriptors) == -1)
			exit_error("Error al crear pipe para redirección de entrada", 47);
		if (dup2(pipeDescriptors[1], STDOUT_FILENO) == -1)
			exit_error("Error al redirigir salida en caso de fallo en la entrada", 50);
		if (close(pipeDescriptors[0]) == -1 || close(pipeDescriptors[1]) == -1)
			exit_error("Error al cerrar pipe para redirección", 51);
		while (command->arg[argIndex])
			command->arg = remove_argument_at_index(command->arg, argIndex);
		free(fileName);
		return (1);
	}
	else if (dup2(fileDescriptor, STDIN_FILENO) == -1)
		exit_error("Error al redirigir entrada", 45);
	close(fileDescriptor);
	free(fileName);
	return (0);
}

char    **remove_argument_at_index(char **argumentList, int removalIndex)
{
    int     totalArgs;
    int     i, newIndex;
    char    **newArgumentList;

    if (argumentList == NULL)
        return NULL;

    totalArgs = 0;
    while (argumentList[totalArgs])
        totalArgs++;

    if (removalIndex < 0 || removalIndex >= totalArgs)
        return argumentList;

    free(argumentList[removalIndex]);

    newArgumentList = malloc(sizeof(char *) * totalArgs);
    if (!newArgumentList)
        exit_error("Error de asignación de memoria al remover argumento", 1);

    newIndex = 0;
    for (i = 0; i < totalArgs; i++)
    {
        if (i == removalIndex)
            continue;
        newArgumentList[newIndex] = argumentList[i];
        newIndex++;
    }
    newArgumentList[newIndex] = NULL;

    free(argumentList);
    return newArgumentList;
}
