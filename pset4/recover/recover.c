#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

// Number of bites in one block
const int BLOCK_SIZE = 512;

// Prototypes
int recognize_header(BYTE *block);

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open raw file
    FILE *raw = fopen(argv[1], "r");
    if (raw == NULL)
    {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }

    // Current file being written
    FILE *output = NULL;

    // Count files being written
    int count = -1;

    // Read raw block by block
    BYTE *buffer = malloc(sizeof(BYTE) * BLOCK_SIZE);
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, raw) == BLOCK_SIZE)
    {
        if (recognize_header(buffer))
        {
            // Close last file
            if (output != NULL) fclose(output);

            count ++;

            // Open new file
            char *file_name = malloc(sizeof(char) * 8);
            sprintf(file_name, "%03i.jpg", count);
            output = fopen(file_name, "w");

            // Write first block
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output);

            free(file_name);
        }
        else
        {
            // The first block have not been read
            if (count == -1) continue;

            // Continue write blocks
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output);
        }
    }

    // Close files and free Memories
    fclose(raw);
    free(buffer);
}

int recognize_header(BYTE *block)
{
    return
        (block[0] == 0xff) &&
        (block[1] == 0xd8) &&
        (block[2] == 0xff) &&
        ((block[3] & 0xf0) == 0xe0);
}