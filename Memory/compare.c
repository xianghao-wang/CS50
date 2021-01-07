#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    char *s = get_string("First name: ");
    char *t = get_string("Second name: ");

    if (strcmp(s, t) == 0)
    {
        printf("Same.\n");
    }
    else
    {
        printf("Different.\n");
    }

    return 0;
}