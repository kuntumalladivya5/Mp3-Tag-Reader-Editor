#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

typedef struct
{
    char title[100];
    char artist[100];
    char album[100];
    char year[10];
    char music[100];
    char comment[100];
} ID3v2Tag;

FILE *open_file(const char *filename);
void read_tag(FILE *fp, char *tag);
unsigned int read_size(FILE *fp);
void print_tag(const char *tag, const char *data);
void view_tag(char *filename);

#endif 