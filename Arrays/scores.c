#include <stdio.h>
#include <cs50.h>

const int TOTAL = 3;

float average(int length, int array[]);

int main(void) {
    int scores[TOTAL];

    for (int i = 0; i < TOTAL; i ++) {
        scores[i] = get_int("%i. Score: ", i + 1);
    }

    printf("Average: %f\n", average(TOTAL, scores));

    return 0;
}

float average(int length, int array[]) {
    int sum = 0;
    for (int i = 0; i < length; i ++) {
        sum += array[i];
    }
    return (float) sum / length;
}