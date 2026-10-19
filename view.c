#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"

/* Open MP3 file */
FILE *open_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return NULL;
    }

    return fp;
}

/* Read 4-byte tag */
void read_tag(FILE *fp, char *tag)
{
    fread(tag, 1, 4, fp);
    tag[4] = '\0';
}

/* Read frame size */
unsigned int read_size(FILE *fp)
{
    unsigned char buffer[4];

    fread(buffer, 1, 4, fp);

    return ((unsigned int)buffer[0] << 24) |
           ((unsigned int)buffer[1] << 16) |
           ((unsigned int)buffer[2] << 8) |
           buffer[3];
}

/* Print tag information */
void print_tag(const char *tag, const char *data)
{
    if (strcmp(tag, "TIT2") == 0)
    {
        printf("Title       : %s\n", data);
    }
    else if (strcmp(tag, "TPE1") == 0)
    {
        printf("Artist      : %s\n", data);
    }
    else if (strcmp(tag, "TALB") == 0)
    {
        printf("Album       : %s\n", data);
    }
    else if (strcmp(tag, "TYER") == 0)
    {
        printf("Year        : %s\n", data);
    }
    else if (strcmp(tag, "TCON") == 0)
    {
        printf("Content     : %s\n", data);
    }
    else if (strcmp(tag, "COMM") == 0)
    {
        printf("Comment     : %s\n", data);
    }
}

/* View MP3 tags */
void view_tag(char *filename)
{
    FILE *fp;
    char header[10];
    char tag[5];
    unsigned int size;
    char *data;
    int i;

    fp = open_file(filename);

    if (fp == NULL)
        return;

    /* Read ID3 header */
    fread(header, 1, 10, fp);

    if (strncmp(header, "ID3", 3) != 0)
    {
        printf("ERROR: Invalid MP3 file\n");
        fclose(fp);
        return;
    }

    printf("\n========== MP3 TAG READER ==========\n");

    printf("\nMP3 TAG INFORMATION\n");
    printf("-------------------------\n");

    /* Read first 6 frames */
    for (i = 0; i < 6; i++)
    {
        read_tag(fp, tag);

        size = read_size(fp);

        /* Skip flags */
        fseek(fp, 2, SEEK_CUR);

        if (size == 0 || size > 1000)
            break;

        data = (char *)malloc(size + 1);

        if (data == NULL)
        {
            printf("ERROR: Memory allocation failed\n");
            fclose(fp);
            return;
        }

        fread(data, 1, size, fp);

        data[size] = '\0';

        /* Skip encoding byte */
        if (size > 1)
        {
            print_tag(tag, data + 1);
        }

        free(data);
    }

    printf("-------------------------\n");

    fclose(fp);

    printf("\n====================================\n");
} 