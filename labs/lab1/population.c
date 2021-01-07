#include <cs50.h>
#include <stdio.h>

int get_available_number(string label, int minimum)
{
    int n;
    do
    {
        n = get_int("%s: ", label);
    }
    while (n < minimum);

    return n;
}

int main(void)
{
    // TODO: Prompt for start size
    int start = get_available_number("Start size", 9);
    // TODO: Prompt for end size
    int end = get_available_number("End size", start);
    // TODO: Calculate number of years until we reach threshold
    int duration = 0;
    while (start < end)
    {
        start = start + start / 3 - start / 4;
        duration += 1;
    }
    // TODO: Print number of years
    printf("Years: %d\n", duration);

    return 0;
}
