/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/20 16:46:17 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** process_redirections:
**   Recorre el arreglo de argumentos del comando, identifica y procesa los
**   operadores de redirección mediante las funciones auxiliares actualizadas.
**
**   Para cada redirección encontrada:
**     - Se ajusta el argumento que contiene el operador y su operando usando
**       extract_filename_from_arg.
**     - Se elimina del arreglo los elementos ya procesados.
**     - Se obtiene el tipo de redirección (get_redirect_type) y se ejecuta la acción:
**          - Para salida (tipos 1, 2, 5, 6): se llama a handle_output_redirection.
**          - Para entrada simple (tipo 3): se llama a redirect_input_from_file.
**          - Para here-document (tipo 4): se llama a handle_here_document.
*/
void	process_redirections(t_cmd *cmd)
{
	int		i;
	char	*file;
	int		rtype;

	i = 0;
	// Mientras se encuentre un operador de redirección en los argumentos...
	while (find_first_redirect_index(cmd->arg + i) != -1)
	{
		// Encuentra el índice relativo del siguiente operador y lo suma a i.
		i += find_first_redirect_index(cmd->arg + i);
		
		// Ajusta el argumento en la posición i (contiene el operador)
		// extrayendo y separando el operador y su operando.
		cmd->arg = extract_filename_from_arg(cmd->arg, i,
				get_operator_for_type(get_redirect_type(cmd->arg[i])), 1);
		
		// Si tras el procesamiento no se detecta un operador, se incrementa i.
		if (!get_redirect_type(cmd->arg[i]))
			i++;
		
		// Si el siguiente argumento también contiene un operador, lo procesa.
		if (get_redirect_type(cmd->arg[i + 1]))
			cmd->arg = extract_filename_from_arg(cmd->arg, i + 1,
				get_operator_for_type(get_redirect_type(cmd->arg[i + 1])), 1);
		
		// Extrae el nombre del archivo, eliminando las comillas si las hay.
		file = str_noquotes(cmd->arg[i + 1]);
		
		// Determina el tipo de redirección en el argumento i.
		rtype = get_redirect_type(cmd->arg[i]);
		if (rtype == 1 || rtype == 2 || rtype == 5 || rtype == 6)
			handle_output_redirection(rtype, cmd, file);
		else if (rtype == 3 && redirect_input_from_file(cmd, file, i))
			return ;
		else if (rtype == 4)
			handle_here_document(cmd, file);
		
		// Elimina del arreglo los elementos ya procesados: el operador y el archivo.
		cmd->arg = remove_argument_at_index(cmd->arg, i);
		cmd->arg = remove_argument_at_index(cmd->arg, i);
	}
}