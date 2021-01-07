// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t Byte;
typedef int16_t Sample;

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

// Prototypes
void copy_header(FILE *source, FILE *destination, int size);
void handle_samples(FILE *source, FILE *destination, float factor);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    copy_header(input, output, HEADER_SIZE);

    // TODO: Read samples from input file and write updated data to output file
    handle_samples(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

void copy_header(FILE *source, FILE *destination, int size)
{
    Byte *header = malloc(sizeof(Byte) * size);
    fread(header, sizeof(Byte), size, source);
    fwrite(header, sizeof(Byte), size, destination);
    free(header);
}


void handle_samples(FILE *source, FILE *destination, float factor)
{
    Sample buffer;
    while (fread(&buffer, sizeof(Sample), 1, source))
    {
        Sample new_sample = buffer * factor;
        fwrite(&new_sample, sizeof(Sample), 1, destination);
    }
}