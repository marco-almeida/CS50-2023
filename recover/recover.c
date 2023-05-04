#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int const BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // check number of arguments
    if (argc != 2)
    {
        printf("Usage: %s [raw file]\n", argv[0]);
        return 1;
    }

    // Remember filenames
    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    // init buffer of size 512
    uint8_t buffer[BLOCK_SIZE];

    // prepare file naming scheme
    int found_jpegs = 0;
    char *filename = malloc(8);

    FILE *img = NULL;

    // starting reading card, 512 by 512 bytes
    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        // if start of new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            sprintf(filename, "%03i.jpg", found_jpegs);
            found_jpegs++;
            printf("File found: %s\n", filename);

            // if its the first jpeg found
            if (found_jpegs - 1 == 0)
            {
                img = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
            else
            {
                fclose(img);
                img = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
        else
        {
            // if we have a jpeg open
            if (found_jpegs > 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
    }

    // close last jpeg
    free(filename);
    fclose(inptr);
    fclose(img);
}

