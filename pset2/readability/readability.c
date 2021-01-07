#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

// Calculate Coleman-Liau index
int cl_index(float l, float s);

// Count the letters
int letter_count(string text);

// Count the words
int word_count(string text);

// Count the sentences
int sentence_count(string text);

int main(void) {
    string text = get_string("Text: ");

    // Get the index
    int letters_n = letter_count(text);
    int words_n = word_count(text);
    int sentences_n = sentence_count(text);
    float l = 100.0 * letters_n / words_n;
    float s = 100.0 * sentences_n / words_n;
    int index = cl_index(l, s);

    // Test
    // printf("%i letter(s)\n", letters_n);
    // printf("%i word(s)\n", words_n);
    // printf("%i sentence(s)\n", sentences_n);

    // Print the result
    if (index >= 16) {
        printf("Grade 16+\n");
    } else if (index < 1) {
        printf("Before Grade 1\n");
    } else {
        printf("Grade %i\n", index);
    }

    return 0;
}

int cl_index(float l, float s) {
    int index = round(0.0588 * l - 0.296 * s - 15.8);
    return index;
}

int letter_count(string text) {
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i ++) {
        if (isalpha(text[i])) count ++;
    }

    return count;
}

int word_count(string text) {
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i ++) {
        if (text[i] == ' ') count ++;
    }

    // There is no white-space going after the last word
    return count + 1;
}

int sentence_count(string text) {
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i ++) {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') count ++;
    }

    return count;
}
