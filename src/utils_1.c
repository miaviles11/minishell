int is_line_empty(char *s)
{
    int i = 0;
    int whitespace_count = 0;

    /* Si la cadena es NULL o está vacía, se considera vacía */
    if (!s || s[0] == '\0')
        return 1;

    /* Se recorre la cadena contando los caracteres en blanco */
    while (s[i])
    {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' ||
            s[i] == '\v' || s[i] == '\f' || s[i] == '\r')
            whitespace_count++;
        i++;
    }
    
    /* Si todos los caracteres son espacios en blanco, la línea está vacía */
    if (whitespace_count == i)
        return 1;
    
    return 0;
}

int is_builtin_no_pipeline(t_cmd *cmd, char *cmd_name)
{
    /* Si no se proporcionó un nombre de comando, se asume que no es built-in */
    if (!cmd_name)
        return 0;
    
    /* Se comparan los nombres con los built-ins que afectan al entorno.
       Se utiliza ft_strncmp */
    if (ft_strncmp(cmd_name, "cd", 3) == 0)
        return 1;
    else if (ft_strncmp(cmd_name, "exit", 5) == 0)
        return 1;
    else if (ft_strncmp(cmd_name, "export", 7) == 0)
        return 1;
    else if (ft_strncmp(cmd_name, "unset", 6) == 0)
        return 1;
    
    /* Otros comandos, incluso si son built-in (como echo, env, pwd),
       pueden ejecutarse en un pipeline */
    return 0;
}