#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

const int KEY_LENGTH = 26;

// Get whether the key is valid
bool key_validity(string key);

// Cipher text
void print_cipher_text(string text, string key);

int main(int argc, string argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else if (!key_validity(argv[1]))
    {
        printf("Key must contain %i non-repeating letters.\n", KEY_LENGTH);
        return 1;
    }

    // Get key
    string key = argv[1];

    // Get input
    string text = get_string("plaintext: ");

    // Cipher input and print it
    printf("ciphertext: ");
    print_cipher_text(text, key);
    printf("\n");

    return 0;
}

bool key_validity(string key)
{
    // Check length
    if (strlen(key) != KEY_LENGTH) return false;

    // Check whether all are letters
    for (int i = 0; i < KEY_LENGTH; i ++)
    {
        if (!isalpha(key[i])) return false;
    }

    // Check whether all letters are non-repeating
    bool is_exist[KEY_LENGTH];
    for (int i = 0; i < KEY_LENGTH; i ++)
    {
        is_exist[i] = false;
    }

    for (int i = 0; i < KEY_LENGTH; i ++)
    {
        is_exist[toupper(key[i]) - 'A'] = true;
    }

    for (int i = 0; i < KEY_LENGTH; i ++)
    {
        if (!is_exist[i]) return false;
    }

    return true;
}

void print_cipher_text(string text, string key)
{
    for (int i = 0, n = strlen(text); i < n; i ++)
    {
        char c = text[i];

        if (isalpha(c))
        {
            // Distance between the original letter and the cipherd letter
            int offset = toupper(key[toupper(c) - 'A']) - toupper(c);
            c += offset;
        }

        printf("%c", c);
    }
}