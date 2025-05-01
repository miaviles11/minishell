/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:17:45 by miaviles          #+#    #+#             */
/*   Updated: 2025/03/17 19:24:53 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * handle_no_newline:
 *   detecta la opción -n (uno o más 'n') en arg[0].
 *   devuelve 1 para saltar ese argumento y pone no_newline=1.
 */
int handle_no_newline(t_cmd *cmd, int *no_newline)
{
    int i = 0;
    int j;

    *no_newline = 0;
    if (cmd->arg[0] && cmd->arg[0][0] == '-')
    {
        j = 1;
        while (cmd->arg[0][j] == 'n')
            j++;
        if (j > 1 && cmd->arg[0][j] == '\0')
        {
            *no_newline = 1;
            i = 1;
        }
    }
    return (i);
}

/*
 * print_arguments:
 *   imprime cada cmd->arg[i] con ft_printf("%s", …),
 *   separando por espacios.
 */
static void print_arguments(t_cmd *cmd, int start)
{
    int i = start;
    while (cmd->arg[i])
    {
        ft_printf("%s", cmd->arg[i]);
        if (cmd->arg[i + 1])
            ft_printf(" ");
        i++;
    }
}

/*
 * minishell_echo:
 *  - sin args       → sólo newline
 *  - -n             → sin newline final
 *  - args sin modificar (el parser ya lleva la expansión/quotes)
 */
int minishell_echo(t_msh *msh)
{
    t_cmd *cmd = msh->cmd;
    int    no_newline;
    int    i;

    if (!cmd || !cmd->arg || !cmd->arg[0])
    {
        ft_printf("\n");
        return (0);
    }

    /* 1) comprueba -n */
    i = handle_no_newline(cmd, &no_newline);

    /* 2) imprime todos los argumentos, tal cual los recibió */
    print_arguments(cmd, i);

    /* 3) newline si no hubo -n */
    if (!no_newline)
        ft_printf("\n");

    return (0);
}

/*int main(int argc, char **argv)
{
	t_msh *msh;

	msh = malloc(sizeof(t_msh));
	msh->cmd = malloc(sizeof(t_cmd));
	ft_printf("[1]: ");
	msh->cmd->arg = (char *[]) {"echo", "Hello", "World", NULL};
	minishell_echo(msh);
	ft_printf("[2]: ");
	msh->cmd->arg = (char *[]) {"echo", "-nnnnnnn", "World", NULL};
	minishell_echo(msh);
	ft_printf("[3]: ");
	msh->cmd->arg = (char *[]) {"echo", "-nnnnnnn 3 -nn", "World", "-nn", NULL};
	minishell_echo(msh);
	ft_printf("[4]: ");
	msh->cmd->arg = (char *[]) {"echo", "- ", NULL};
	minishell_echo(msh);
	return (0);
}*/
