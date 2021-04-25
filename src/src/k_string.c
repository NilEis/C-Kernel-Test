#include <stddef.h>
#include <stdint.h>

int k_strlen(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}