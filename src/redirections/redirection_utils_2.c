/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carlsanc <carlsanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:14:06 by carlsanc          #+#    #+#             */
/*   Updated: 2025/03/20 16:46:17 by carlsanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


char **insert_argument_at_index(char **argList, char *newArg, int idx)
{
    int count = count_arguments(argList), i = 0, j;
    char **newList;
    if (idx < 0)
        idx = 0;
    if (idx > count)
        idx = count;
    newList = malloc(sizeof(char *) * (count + 2));
    if (!newList)
        exit_error("Error de asignación en insert_argument_at_index", 1);
    while (i < idx)
    {
        newList[i] = argList[i];
        i++;
    }
    newList[idx] = newArg;
    j = idx;
    while (i < count)
    {
        newList[++j] = argList[i++];
    }
    newList[count + 1] = NULL;
    free(argList);
    return newList;
}
int	count_arguments(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

static char **keep_argument(char **arg, int j, char c)
{
    char *temp;

    arg = insert_argument_at_index(arg, 
            ft_substr(arg[j], 0, get_next_diff_op(0, arg[j])), j + 1);
    temp = ft_strdup(ft_strchr(arg[j], c));
    free(arg[j]);
    arg[j] = temp;
    return (get_filename(arg, j, c, 2));
}
static char **get_filename(char **args, int index, char opChar, int offset)
{
    int i;
    int k;

    i = 0;
    k = offset;
    if (args[index][i] == '2' && args[index][i + 1] == '>')
        i++;
    while (args[index][i] == opChar)
        i++;
    if (!args[index][i])
        return (args);
    if (i == 0)
        return (keep_argument(args, index, opChar));
    args = insert_argument_at_index(args, ft_substr(args[index], 0, i),
                                    index + k);
    k++;
    args = insert_argument_at_index(args, ft_substr(args[index], i,
                                    get_next_diff_op(i, args[index]) - i),
                                    index + k);
    k++;
    i = get_next_diff_op(i, args[index]);
    if (args[index][i])
    {
        args = insert_argument_at_index(args, ft_substr(args[index], i,
                                    ft_strlen(args[index]) - i),
                                    index + k);
        k++;
    }
    args = remove_argument_at_index(args, index);
    return (args);
}
int	get_next_diff_op(int i, char *str)
{
	while (str[i] && !isdifoperator(str[i]))
		i++;
	return (i);
}