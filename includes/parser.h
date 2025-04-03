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

int	count_pipes(const char *inputLine);
char	**split_pipes(char *inputLine);
int	parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine);
int	validate_and_split_input(t_msh *shell, char *inputLine, char ***segments);
int	check_quotes_balance(const char *s, t_msh *shell);
char	**extract_pipe_segments(char *s, char **segments);
int	validate_pipe_segment(char *segment);
int	validate_redirection_syntax(char **segments, int index);
int	has_variable(const char *s);
char	*substitute_variables(t_cmd *cmd, char *s, char *varReminder);
char	*join_special(const char *s1, const char *s2);
int	needs_home_expansion(const char *s);
char	*expand_home_directory(char *s);
int	check_variable_and_digit(const char *s);
char	*quit_variable_and_digit(char *s, int dummy1, int dummy2);
int	is_digit_special(int c);
char	*substitute_variable_value(t_cmd *cmd, char *line, char *varReminder);
char	*replace_special_value(char *s);
char	*replace_special_value_helper(char *s, int i, int j);
char	*extract_variable_name(char *line);
int	find_next_dollar(const char *s, int start);
char	*split_variable_reminder(char *line, int index, t_cmd *cmd);
int	special_char_check(char c);
char	*compare_variable_name(t_cmd *cmd, char *line, char *nameVar);
int	compare_env_variable_name(const char *s1, const char *s2);
char	*extract_env_value(const char *envVar);
char	*replace_variable_in_line(char *line, char *var);
void	perform_expansion(t_cmd **command);
t_cmd	*get_last_command_node(t_cmd **cmd);

#endif