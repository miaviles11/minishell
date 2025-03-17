/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:41:28 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/15 18:12:52 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

static int	count_valid_pipes(const char *inputLine);
int	count_pipes(const char *inputLine);
static int	skip_quoted_segment(const char *s, int i, char quote);
char	**split_pipes(char *inputLine);
int	parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine);
int	validate_and_split_input(t_msh *shell, char *inputLine, char ***segments);
int	check_quotes_balance(const char *s, t_msh *shell);
static int	both_quotes_exist(const char *s, int *i, char quote);
char	**extract_pipe_segments(char *s, char **segments);
int	validate_pipe_segment(char *segment);
int	validate_redirection_syntax(char **segments, int index);

#endif