#include <stdio.h>
#include <cs50.h>

int card_number_length(long card_number)
{
    int n = 0;
    while (card_number > 0)
    {
        n += 1;
        card_number /= 10;
    }
    return n;
}

int first_2_digits(long card_number)
{
    int length = card_number_length(card_number);
    for (int i = 0; i < length - 2; i ++)
    {
        card_number /= 10;
    }

    return card_number;
}

bool is_valid_card(long card_number)
{
    int checksum = 0;

    // Flag represents whether multiply the current digit by 2
    bool is_multiplied = false;
    while (card_number > 0)
    {
        // Current digit
        int digit = card_number % 10;

        // Calculate checksum
        if (is_multiplied)
        {
            int x = digit * 2;
            int y = x % 10 + x / 10;
            checksum += y;
        }
        else
        {
            checksum += digit;
        }

        card_number /= 10;
        is_multiplied = !is_multiplied;
    }

    return checksum % 10 == 0;
}

int main(void)
{
    long card_number;

    // Get the card number
    card_number = get_long("Number: ");

    // Decide whether the card is valid
    if (is_valid_card(card_number))
    {
        // The first 2 digits of the card number
        int prefix = first_2_digits(card_number);
        // The length of the card number
        int length = card_number_length(card_number);

        // Decide the company
        if ((prefix == 34 || prefix == 37) && (length == 15))
        {
            // AMEX
            printf("AMEX\n");
        }
        else if ((prefix >= 51 && prefix <= 55) && (length == 16))
        {
            // MASTERCARD
            printf("MASTERCARD\n");
        }
        else if ((prefix / 10 == 4) && (length == 13 || length == 16))
        {
            // VISA
            printf("VISA\n");
        }
        else
        {
            // INVALID
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }


    return 0;
}