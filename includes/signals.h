/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:28:50 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/18 18:41:11 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

void	setup_signals(void);
void setup_child_signals(void);
void handle_parent_signal(int sign);
void handle_child_signal(int sign);
#endif