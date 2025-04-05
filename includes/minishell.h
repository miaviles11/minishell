/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:19:16 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 18:53:56 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

# define WHITE_T "\033[1;37m"
# define RESET_COLOR "\033[0m"

/*
** t_pipe: Estructura para el manejo de pipes.
** Se utiliza para almacenar dos conjuntos de descriptores (para alternar entre ellos),
** además de las copias de los descriptores estándar (in, out) y flags de estado.
*/
typedef struct s_pipe
{
	int	fd[2][2];
	int	in;
	int	out;
	int	last;
	int	out_error;
}				t_pipe;


typedef struct s_flags
{
	int	quote;          // Indica si hay error en el balance de comillas
	int	pipe;           // Número de pipes detectados en la línea
	int	redic;          // Indicador de redirecciones
	int	dollar_special; // Indicador para casos especiales con $
}	t_flags;

/*
** t_cmd: Nodo de comando.
** Cada nodo contiene el nombre del comando, su arreglo de argumentos y
** el número de argumentos. La información global (entorno, flags, pipes)
** se encuentra en la estructura principal y se pasa como argumento a las funciones.
*/

typedef struct s_cmd
{
	char 		*cmd;
	char 		**arg;
	int 		num_arg;
	char 		*input_file;
	char 		*output_file;
	int 		background;
	t_flags		*flags; // Agregar los flags aquí
	struct 		s_cmd *next;
}	t_cmd;

/*
** t_msh: Estructura principal del shell.
** Se integran aquí tanto el entorno como las banderas globales, ya que estos
** datos se usan en toda la ejecución y son comunes a todos los comandos.
*/
typedef struct s_msh
{
	int		bash_lvl;
	int		total_chars;
	t_cmd	*cmd;
	char	**env;      // Array de variables de entorno (cada cadena "VAR=VAL")
	char	*path;      // Contenido de la variable PATH, extraído del env
	int		num_env;    // Número de variables en env
}				t_msh;

# include "../libft/libft.h"
# include "env.h"
# include "parser.h"
# include "redirections.h"
# include "signals.h"
# include "executor.h"

# include "builtins.h"

int 	is_builtin_no_pipeline(t_cmd *cmd, char *cmd_name);
char	*str_noquotes(char *str);
void	run_shell_loop(t_msh *shell);
void	put_error(char *bash, char *file, char *error);
void	exit_error(char *str, int n);
//int	main(int argc, char **argv, char **envp);

#endif