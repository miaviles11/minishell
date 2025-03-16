/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/15 13:14:06 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** check_quotes_balance:
**   Verifica que en la cadena 's' las comillas dobles y simples estén balanceadas.
**   Retorna 1 si están balanceadas; de lo contrario, retorna 0.
**   Además, en caso de error, actualiza el campo 'quote' de la estructura shell.
**
** Nota: Se utiliza la función auxiliar both_quotes_exist() para procesar
**       la apertura/cierre de cada tipo de comilla.
*/
int	check_quotes_balance(const char *s, t_msh *shell)
{
	int	i;
	int	simple_count;
	int	double_count;

	i = 0;
	simple_count = 0;
	double_count = 0;
	while (s[i])
	{
		if (s[i] == '"' )
			double_count += both_quotes_exist(s, &i, '"');
		else if (s[i] == '\'')
			simple_count += both_quotes_exist(s, &i, '\'');
		// Si alguna cuenta es impar, las comillas no están balanceadas.
		if ((double_count % 2) != 0 || (simple_count % 2) != 0)
		{
			shell->quote = 1;
			return (0);
		}
		i++;
	}
	return (1);
}
static int	both_quotes_exist(const char *s, int *i, char quote)
{
	int	pairing;

	pairing = 1;
	// Avanza mientras haya caracteres y no se encuentre la comilla de cierre.
	while (s[*i + 1] && s[*i + 1] != quote)
		(*i)++;
	// Si se encontró la comilla de cierre, incrementa el contador.
	if (s[*i + 1] == quote)
		pairing++;
	// Salta la comilla de cierre (o el final si no se encontró).
	(*i)++;
	return (pairing);
}
