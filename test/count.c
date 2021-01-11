#include <stdbool.h>
#include <stdio.h>

typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./count INPUT\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int count = 0;
    int sequence_size = 0;

    while (true)
    {
        BYTE b;
        fread(&b, 1, 1, file);
        printf("%x\n", b);

        if ((b & 0x80) == 0)
        {
            sequence_size = 1;
        }
        else if ((b & 0xE0) == 0xC0)
        {
            sequence_size = 2;
        }
        else if ((b & 0xF0) == 0xE0)
        {
            sequence_size = 3;
        }
        else if ((b & 0xF8) == 0xF0)
        {
            sequence_size = 4;
        }

        count ++;
        sequence_size -= 1;

        while (sequence_size > 0)
        {
            fread(&b, 1, 1, file);
            printf("%x\n", b);
            sequence_size -= 1;
        }

        if (feof(file))
        {
            break;
        }

    }

    fclose(file);
    printf("Number of characters: %i\n", count);
}