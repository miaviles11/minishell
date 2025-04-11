#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include	<fcntl.h>

int		is_redirect_operator(char c);
int		get_redirect_type(char *str);
int		find_first_redirect_index(char **args);
char	get_operator_for_type(int n);
int		find_next_redirect_operator_index(int i, char *str);
void	process_redirections(t_cmd *cmd);
static char	**extract_filename_from_arg(char **args, int index, char opChar, int offset);
void	handle_output_redirection(int redirType, t_cmd *command, char *filename);
void	handle_here_document(t_cmd *command, char *delimiter);
int	redirect_input_from_file(t_cmd *command, char *fileName, int argIndex);
char    **remove_argument_at_index(char **argumentList, int removalIndex);
char	**insert_argument_at_index(char **argList, char *newArg, int idx);
char	*keep_argument(char *argument);
int	count_arguments(char **args);
int	get_next_diff_op(int i, char *str);
static char **get_filename(char **args, int index, char opChar, int offset);

#endif