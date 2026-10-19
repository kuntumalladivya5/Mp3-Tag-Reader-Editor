#ifndef EDIT_H
#define EDIT_H

typedef struct
{
    char *tag;
    char *new_data;
    char *filename;

} EditInfo;

int edit_tag(EditInfo *editInfo);

unsigned int convert_big_endian(unsigned char *buffer);

void convert_to_big_endian(unsigned int size,
                           unsigned char *buffer);

#endif 