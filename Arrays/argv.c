#include <stdio.h>
#include <cs50.h>

int main(int argc, string args[]) {
    for (int i = 0; i < argc; i ++) {
        printf("Argument %i: %s\n", argc, args[i]);
    }

    return 0;
}