/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:41:28 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/15 12:41:28 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_cmd
{
    char    *cmd;         // Nombre del comando
    char    **args;       // Argumentos
    char    *infile;      // Archivo de entrada ('<' o '<<')
    char    *outfile;     // Archivo de salida ('>' o '>>')
    int     append;       // Flag para '>>' (modo append)
    int     heredoc;      // Flag para '<<'
    int     pipe;         // Flag para '|'
    struct s_cmd *next;   // Siguiente comando en pipeline
} t_cmd;

#endif