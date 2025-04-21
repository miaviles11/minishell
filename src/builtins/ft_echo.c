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

int handle_no_newline(t_cmd *cmd, int *no_newline)
{
	int i;
	int j;

	i = 0;
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

void	print_arguments(t_cmd *cmd, int i)
{
	while (cmd->arg[i] != NULL)
	{
		ft_printf("%s", cmd->arg[i]);
		if (cmd->arg[i + 1])
			ft_printf(" ");
		i++;
	}
}

int	minishell_echo(t_msh *msh)
{
	t_cmd	*cmd;
    int		i;
	int		j;
    int		no_newline;
	char 	*temp;

    cmd = msh->cmd;
    // Validar que cmd->arg no sea NULL
    if (!cmd || !cmd->arg || !cmd->arg[0])
    {
        ft_printf("\n");
        return (0);
    }
	j = 0;
	while (cmd->arg[j])
	{
		temp = str_noquotes_expand(cmd->arg[j], msh);
		if (temp) 
		{
			free(cmd->arg[j]);  // Liberar memoria del argumento original
			cmd->arg[j] = temp;  // Reemplazar con la versión sin comillas
		}
		j++;
	}
	// Manejar la opción -n
    i = handle_no_newline(cmd, &no_newline);
    // Imprimir los argumentos restantes
    print_arguments(cmd, i);
    // Imprimir un salto de línea si no se especificó -n
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
