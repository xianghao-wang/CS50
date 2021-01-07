#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Great User
    string answer = get_string("What is your name?\n");
    printf("hello, %s\n", answer);

    return 0;
}