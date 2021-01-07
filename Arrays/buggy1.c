#include <stdio.h>
#include <cs50.h>

// Prototype
int get_negative_int();

int main(void) {
    // Get negative integer from user
    int n = get_negative_int();
    printf("%i\n", n);

    return 0;
}

int get_negative_int() {
    int n;

    do {
        n = get_int("Negative integer: ");
    } while (n >= 0);

    return n;
}