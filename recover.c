#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define max file length.
#define FILE_LEN 8
#define MAX_FILE_NO 999

// Function prototypes.
bool check_header(unsigned char bytes[512]);

int main(int argc, char *argv[])
{
    // Declare varibales.
    char fileName[FILE_LEN];
    unsigned char bytes[512];
    bool jpg_found;
    int count = 0;
    FILE *write_file = NULL;

    // Usage check. If no file argument provided at prompt, display error message and quit program.
    if (argc != 2)
    {
        printf("Usage: ./recover image.\n");
        return 1;
    }

    // Open file to read data from.
    FILE *read_file = fopen(argv[1], "r");

    // Error check the file pointer is OK.
    if (read_file == NULL)
    {
        printf("There was an error trying to open the file.\n");
        return 1;
    }

    while (true)
    {
        // Read first block of 512 bytes into buffer.
        fread(bytes, 512, 1, read_file);

        // Check if we're at the end of the input file.
        if (feof(read_file))
        {
            break;
        }

        // If not end of file, continue and check bytes 0-2 inclusive for JPG header.
        jpg_found = check_header(bytes);

        // If another JPG detected and there is a current file being written, close the file.
        if (jpg_found && write_file != NULL)
        {
            fclose(write_file);
            count++;
        }

        // If JPG found, open file to write to.
        if (jpg_found)
        {
            sprintf(fileName, "%03i.jpg", count);
            write_file = fopen(fileName, "w");
        }

        // If write file is open, write data.
        if (write_file != NULL)
        {
            fwrite(bytes, 512, 1, write_file);
        }
    }

    // Close off last JPG write file.
    fclose(write_file);

    // Close off 'read file'.
    fclose(read_file);

    return 0;

}

// Function to check header bytes and detect start of new JPG.
bool check_header(unsigned char bytes[512])
{
    if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff)
    {
        if ((bytes[3] & 0xef) == 0xe0)
        {
            return true;
        }
    }
    return false;
}

