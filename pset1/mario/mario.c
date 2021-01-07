#include <stdio.h>
#include <cs50.h>

int get_available_number(string label, int mini, int max) {
    int n;
    do {
        n = get_int("%s: ", label);
    } while (n < mini || n > max);

    return n;
}

void put_cells(char c, int times) {
    for (int i = 0; i < times; i ++) {
        printf("%c", c);
    }
}

int main(void) {
    // Get the height
    int h = get_available_number("Height", 1, 8);

    // Draw
    for (int i = 0; i < h; i ++) {
        // Left gap
        int left_gap = h - i - 1;
        put_cells(' ', left_gap);

        // Left block
        put_cells('#', i + 1);

        // Gap
        put_cells(' ', 2);

        // Right block
        put_cells('#', i + 1);

        printf("\n");
    }

    return 0;
}