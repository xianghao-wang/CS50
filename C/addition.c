#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long x = get_int("x: ");
    long y = get_int("y: ");

    printf("%li\n", x + y);

    return 0;
}