#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(void)
{
    // Capitalised a string

    char *s = get_string("s: ");

    // Copy it to string c
    char *c = malloc(strlen(s) + 1);

    // Out of memory
    if (c == NULL)
    {
        return 1;
    }

    // for (int i = 0, n = strlen(s); i <= n; i ++) // The reason why use i<=n: the end of a string is \0, we should also copy it to the new string
    // {
    //     c[i] = s[i];
    // }
    strcpy(c, s);

    if (strlen(s) != 0)
    {
        c[0] = toupper(c[0]);
    }

    // Print result
    printf("s: %s\n", s);
    printf("c: %s\n", c);

    free(c);

    return 0;
}