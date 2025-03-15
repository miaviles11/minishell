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

int	parse_input_line(t_msh *shell, t_cmd **commandList, char *inputLine);
int	validate_and_split_input(t_msh *shell, char *inputLine, char ***segments);
int	check_quotes_balance(const char *s, t_msh *shell);
static int	both_quotes_exist(const char *s, int *i, char quote);

#endif