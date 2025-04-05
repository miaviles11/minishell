/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:41:28 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 18:59:59 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*process_arguments_1.c*/
char	*extract_command(t_msh *shell, char *segment);
int		count_arguments(char *s);

/*process_arguments_2.c*/
int		check_null_args(char *s);
char	*change_null_args(char *s, t_cmd *cmd);
char	**extract_arguments(t_msh *shell, char *segment, t_cmd *command);

/*process_arguments_utils.c*/
char	*quit_null_space(char *s, int index);
char	*quit_null(char *s, int index);

/*expansion_utils_1.c*/
int		has_variable(const char *s);
char	*substitute_variables(t_cmd *cmd, char *s, char **varReminder);
char	*join_special(const char *s1, const char *s2);
int		needs_home_expansion(const char *s);
char	*expand_home_directory(char *s);

/*expansion_utils_2.c*/
int		check_variable_and_digit(const char *s);
char	*quit_variable_and_digit(char *s);
int		is_digit_special(int c);
char	*substitute_variable_value(t_cmd *cmd, char *line, char **varReminder);

/*expansion_utils_3.c*/
char	*replace_special_value(char *s);
char	*replace_special_value_helper(char *s, int i, int j);
char	*extract_variable_name(char *line);
int		find_next_dollar(const char *s, int start);
char	*split_variable_reminder(char *line, int index, t_cmd *cmd);

/*expansion_utils_4.c*/
int		special_char_check(char c);
char	*compare_variable_name(t_cmd *cmd, char *line, char *nameVar);
int		compare_env_variable_name(const char *s1, const char *s2);
char	*extract_env_value(const char *envVar);
char	*replace_variable_in_line(char *line, char *var);

/*p_pipes_utils.c*/
char	**extract_pipe_segments(char *s, char **segments);
int		validate_pipe_segment(char *segment);
int		validate_redirection_syntax(char **segments, int index);

/*parse_pipes.c*/
int		count_pipes(const char *inputLine);
char	**split_pipes(char *inputLine);

/*parse_quotes.c*/
int		check_quotes_balance(const char *s, t_msh *shell);
int		get_next_quote(int i, char *str, char c);

/*parser_functions.c*/
int		validate_and_split_input(t_msh *shell, char *inputLine, char ***segments);
void	perform_expansion(t_cmd **command);
t_cmd	*get_last_command_node(t_cmd **cmd);

/*parser.c*/
int	parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine);

#endif