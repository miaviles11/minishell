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
#define CWD_SIZE 1024
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
	int	dollar_special;
}	t_flags;

/*
** t_cmd: Nodo de comando.
** Cada nodo contiene el nombre del comando, su arreglo de argumentos y
** el número de argumentos. La información global (entorno, flags, pipes)
** se encuentra en la estructura principal y se pasa como argumento a las funciones.
*/

typedef struct s_cmd
{
	char		*cmd;
	char		**arg;
	int			num_arg;
	char		*input_file;
	char		*output_file;
	int			background;
	t_flags		*flags; // Puntero a las banderas globales
	char		**env;  // Puntero al entorno global
	int			error_value;
	struct		s_cmd *next;
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
	char	**env;
	char	*path;
	int		num_env;
	int		error_value;

	int	quote;
	int	pipe;
	int	redic;
}				t_msh;

# include "../libft/libft.h"
# include "parser.h"
# include "redirections.h"
# include "signals.h"
# include "executor.h"

# include "builtins.h"

/* clean_minishell.c */
void	free_command_node(t_cmd *cmd);
void	free_command_list(t_cmd *cmd);
void	cleanup_shell(t_msh *shell);
char	*preprocess_redirections(const char *line);

/* init_shell_utils.c */
int		init_shell_struct(t_msh *shell);
int		init_shell_env(t_msh *shell, char **envp);
int		copy_env_values(t_msh *shell, char **envp);
void	free_env_values(t_msh *shell, int index);
t_msh	*initialize_shell(char **envp);

/* env_utils.c */
char	*get_env_value(t_msh *shell, const char *var_name);

/* shell_level.c */
void	update_shell_level(t_msh *shell);
void	update_shlvl_env(t_msh *shell, int shlvl);

/* env_setter.c */
int		set_env_value(t_msh *shell, const char *var_name, const char *value);
int		check_env_var(char *env_entry, const char *var_name, int len);
int		update_existing_env(t_msh *shell, int i, const char *var_name, 
						const char *value);
int		add_new_env(t_msh *shell, const char *var_name, const char *value);
void	create_env_entry(char *entry, const char *var_name, const char *value);

/* ft_error.c */
void	put_error(char *bash, char *file, char *error);
void	exit_error(char *str, int n);

/* utils1.c */
int	is_line_empty(const char *s);
char	*str_noquotes(char *str);
int	count_args(char **args);
void	init_here_doc(int p[2], int *tty_fd);
void	read_here_doc(int write_fd, int tty_fd, char *delimiter);

/* minishell.c */
int		main(int argc, char **argv, char **envp);

/* shell_loop.c */
void	run_shell_loop(t_msh *shell);
char	*get_input_line(int interactive);
int		process_raw_line(char **raw_line);
void	process_command_line(t_msh *shell, char *line, t_cmd **old_cmd);
void	execute_command(t_msh *shell);

#endif