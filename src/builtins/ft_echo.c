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
 * Comprueba la opción -n (uno o más 'n') en cmd->arg[0].
 * Si está, pone *no_newline = 1 y devuelve 1 para saltar ese arg.
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
 * print_echo_arg:
 * Si el argumento empieza y acaba con ' o ", lo imprime sin esas comillas.
 * En otro caso, lo imprime tal cual.
 */
static void print_echo_arg(char *s)
{
    int len;

    if (!s)
        return;
    len = ft_strlen(s);
    if ((len >= 2 && s[0] == '"'  && s[len - 1] == '"') ||
        (len >= 2 && s[0] == '\'' && s[len - 1] == '\''))
    {
        /* Imprime el contenido entre comillas */
        ft_printf("%.*s", len - 2, s + 1);
    }
    else
    {
        ft_printf("%s", s);
    }
}

/*
 * minishell_echo:
 * - Soporta echo sin args -> solo newline.
 * - Opcional -n (no newline).
 * - Imprime args separados por espacio.
 * - Quita comillas envolventes de cada arg.
 */
int minishell_echo(t_msh *msh)
{
    t_cmd *cmd;
    int    i;
    int    no_newline;

    cmd = msh->cmd;
    /* Si no hay args, solo salto de línea */
    if (!cmd || !cmd->arg || !cmd->arg[0])
    {
        ft_printf("\n");
        return (0);
    }

    /* 1) Verificar -n */
    i = handle_no_newline(cmd, &no_newline);

    /* 2) Imprimir cada argumento */
    while (cmd->arg[i])
    {
        print_echo_arg(cmd->arg[i]);
        if (cmd->arg[i + 1])
            ft_printf(" ");
        i++;
    }

    /* 3) Salto de línea si no hubo -n */
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
