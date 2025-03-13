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
