#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit.h"

/* Convert big endian to integer */
unsigned int convert_big_endian(unsigned char *buffer)
{
    unsigned int size;

    size = ((unsigned int)buffer[0] << 24) |
           ((unsigned int)buffer[1] << 16) |
           ((unsigned int)buffer[2] << 8) |
           ((unsigned int)buffer[3]);

    return size;
}

/* Convert integer to big endian */
void convert_to_big_endian(unsigned int size,
                           unsigned char *buffer)
{
    buffer[0] = (size >> 24) & 0xFF;
    buffer[1] = (size >> 16) & 0xFF;
    buffer[2] = (size >> 8) & 0xFF;
    buffer[3] = size & 0xFF;
}

/* Edit MP3 tag */
int edit_tag(EditInfo *editInfo)
{
    FILE *fp;
    char header[10];
    char tag[5];
    unsigned char size_buffer[4];
    unsigned int size;
    int i;

    fp = fopen(editInfo->filename, "r+b");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return 1;
    }

    /* Read ID3 header */
    if (fread(header, 1, 10, fp) != 10)
    {
        printf("ERROR: Unable to read MP3 header\n");
        fclose(fp);
        return 1;
    }

    if (strncmp(header, "ID3", 3) != 0)
    {
        printf("ERROR: Invalid MP3 file\n");
        fclose(fp);
        return 1;
    }

    /* Search frames */
    for (i = 0; i < 20; i++)
    {
        if (fread(tag, 1, 4, fp) != 4)
            break;

        tag[4] = '\0';

        if (fread(size_buffer, 1, 4, fp) != 4)
            break;

        size = convert_big_endian(size_buffer);

        /* Skip flags */
        fseek(fp, 2, SEEK_CUR);

        if (size == 0 || size > 1000)
            break;

        if ((strcmp(editInfo->tag, "-t") == 0 &&
             strcmp(tag, "TIT2") == 0) ||

            (strcmp(editInfo->tag, "-a") == 0 &&
             strcmp(tag, "TPE1") == 0) ||

            (strcmp(editInfo->tag, "-A") == 0 &&
             strcmp(tag, "TALB") == 0) ||

            (strcmp(editInfo->tag, "-y") == 0 &&
             strcmp(tag, "TYER") == 0) ||

            (strcmp(editInfo->tag, "-m") == 0 &&
             strcmp(tag, "TCON") == 0))
        {
            unsigned char encoding;
            char *buffer;

            /* Read encoding byte */
            if (fread(&encoding, 1, 1, fp) != 1)
            {
                fclose(fp);
                return 1;
            }

            buffer = (char *)calloc(size, sizeof(char));

            if (buffer == NULL)
            {
                printf("ERROR: Memory allocation failed\n");
                fclose(fp);
                return 1;
            }

            buffer[0] = encoding;

            /* Copy new data */
            strncpy(buffer + 1,
                    editInfo->new_data,
                    size - 1);

            /* Go back to beginning of frame data */
            fseek(fp, -1, SEEK_CUR);

            /* Write new data */
            fwrite(buffer, 1, size, fp);

            free(buffer);

            printf("Tag %s is updated successfully\n", tag);

            fclose(fp);
            return 0;
        }

        /* Skip frame data */
        fseek(fp, size, SEEK_CUR);
    }

    printf("ERROR: Tag not found\n");

    fclose(fp);

    return 1;
}