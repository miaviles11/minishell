#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

int	is_redirect_operator(char c);
int	get_redirect_type(char *str);
int	find_first_redirect_index(char **args);
char	get_operator_for_type(int n);
int	find_next_redirect_operator_index(int i, char *str);

#endif