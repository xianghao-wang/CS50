#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string s = "Hi!";
    printf("%c\n", *(s + 10000));

    return 0;
}