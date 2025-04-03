#include "../../../includes/minishell.h"
/*
** replace_special_value:
**   Reemplaza el patrón especial "$?" en la cadena 's' por su valor correspondiente.
**   Utiliza get_next_quote para localizar el signo '?' y, si encuentra el patrón "$?",
**   llama a replace_special_value_helper
**   para efectuar la sustitución.
**
** Retorna la cadena modificada.
*/
char	*replace_special_value(char *s)
{
	int	i;
	int	j;

	// Busca la posición del signo '?' y retrocede 1 para posicionarse en '$'
	i = get_next_quote(0, s, '?') - 1;
	while (s[i])
	{
		// Si se detecta el patrón "$?"
		if (s[i] == '$' && s[i + 1] && s[i + 1] == '?')
			s = replace_special_value_helper(s, i, -1);
		j = i;
		// Busca la siguiente aparición de '$'
		i = get_next_quote(0, s, '$');
		// Si no se ha avanzado, sal del bucle
		if (j == i)
			break ;
	}
	return (s);
}
char	*replace_special_value_helper(char *s, int i, int j)
{
	char	*errorStr;
	char	*prefix;
	char	*suffix;
	char	*result;
	char	*temp;

	// Convierte el valor global g_error a cadena.
	errorStr = ft_itoa(g_error);
	// Extrae el prefijo: todo antes de la ocurrencia de "$?"
	prefix = ft_substr(s, 0, i);
	// Se asume que "$?" ocupa 2 caracteres; extrae el sufijo desde i+2.
	suffix = ft_strdup(s + i + 2);
	// Une el prefijo con el valor de error.
	result = ft_strjoin(prefix, errorStr);
	// Une el resultado anterior con el sufijo.
	temp = result;
	result = ft_strjoin(result, suffix);
	// Libera las cadenas temporales utilizadas.
	free(temp);
	free(prefix);
	free(suffix);
	free(errorStr);
	free(s);
	return (result);
}

/*
** extract_variable_name:
**   Extrae el nombre de la variable de la cadena 'line', comenzando justo después
**   del primer signo '$'. El nombre se extiende hasta encontrar un espacio, una comilla
**   (simple o doble) o un carácter especial (según special_char_check).
**
** Retorna una nueva cadena con el nombre de la variable.
*/
char	*extract_variable_name(char *line)
{
	int		i;
	int		length;
	char	*result;

	// Ubica la posición del primer '$' y avanza un carácter.
	i = get_next_quote(0, line, '$') + 1;
	length = 0;
	// Cuenta la longitud del nombre de la variable hasta encontrar un delimitador.
	while (line[i + length] && line[i + length] != ' ' &&
		   line[i + length] != '"' && line[i + length] != '\'' &&
		   special_char_check(line[i + length]) == 0)
		length++;
	// Reserva memoria para el nombre de la variable (más 1 para el terminador)
	result = ft_calloc(length + 1, sizeof(char));
	if (!result)
		exit_error("Error malloc", 16);
	// Copia el nombre de la variable.
	length = 0;
	while (line[i + length] && line[i + length] != ' ' &&
		   line[i + length] != '"' && line[i + length] != '\'' &&
		   special_char_check(line[i + length]) == 0)
	{
		result[length] = line[i + length];
		length++;
	}
	return (result);
}
/*
** find_next_dollar:
**   Busca en la cadena 's' a partir del índice 'start' la siguiente aparición
**   del carácter '$' que no se encuentre dentro de un bloque de comillas.
**
** Parámetros:
**   s     : La cadena en la que se realiza la búsqueda.
**   start : El índice desde el cual comenzar la búsqueda.
**
** Retorna:
**   El índice de la siguiente ocurrencia de '$' fuera de comillas, o -1 si no se encuentra.
*/
int	find_next_dollar(const char *s, int start)
{
	int	i;

	i = start;
	while (s[i])
	{
		// Si se encuentra una comilla, salta todo el bloque entre comillas.
		if (s[i] == '"' || s[i] == '\'')
		{
			i = get_next_quote(i + 1, (char *)s, s[i]);
			continue;
		}
		// Si se encuentra '$', retorna el índice.
		if (s[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}
char	*split_variable_reminder(char *line, int index, t_cmd *cmd)
{
	int		len;
	int		i;
	char	*res;

	// Indica que se ha detectado un caso especial (caracter especial tras la variable)
	cmd->flags->dollar_special = 1;
	
	// Calcula la longitud de la subcadena desde 'index' hasta el final
	len = 0;
	while (line[index + len])
		len++;
	
	// Reserva memoria para la subcadena (más 1 para el terminador nulo)
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		exit_error("Error malloc", 17);
	
	// Copia la subcadena
	i = 0;
	while (i < len)
	{
		res[i] = line[index + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
