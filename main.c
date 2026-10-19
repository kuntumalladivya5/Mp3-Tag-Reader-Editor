/*Name : KUNTUMALLA DIVYA
Date   :5/10/2026
Description: The MP3 ID3 Tag Reader and Editor is a C-based application used to read and modify metadata information stored inside MP3 audio files.
The project works with ID3 tags such as Title, Artist, Album, Year, Content Type, and Comments.*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "view.h"
#include "edit.h"

void print_usage()
{
    printf("\n========== MP3 TAG READER ==========\n");

    printf("\nVIEW MODE:\n");
    printf("./mp3 -v sample.mp3\n");

    printf("\nEDIT MODE:\n");
    printf("./mp3 -e sample.mp3\n");

    printf("\nHELP:\n");
    printf("./mp3 --help\n");

    printf("\n====================================\n");
}

int main(int argc, char *argv[])
{
    EditInfo editInfo;
    int choice;
    char new_value[100];

    if (argc < 2)
    {
        printf("ERROR: No arguments passed\n");
        print_usage();
        return 1;
    }

    /* HELP */
    if (strcmp(argv[1], "--help") == 0)
    {
        print_usage();
        return 0;
    }

    /* VIEW MODE */
    else if (strcmp(argv[1], "-v") == 0)
    {
        if (argc != 3)
        {
            printf("ERROR: Invalid arguments\n");
            printf("Usage: ./mp3 -v filename.mp3\n");
            return 1;
        }

        view_tag(argv[2]);
    }

    /* EDIT MODE */
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 3)
        {
            printf("ERROR: Invalid arguments\n");
            printf("Usage: ./mp3 -e filename.mp3\n");
            return 1;
        }

        printf("\n========== EDIT MENU ==========\n");
        printf("1. Edit Title\n");
        printf("2. Edit Artist\n");
        printf("3. Edit Album\n");
        printf("4. Edit Year\n");
        printf("5. Edit Content Type\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 6)
        {
            printf("Exiting...\n");
            return 0;
        }

        if (choice < 1 || choice > 5)
        {
            printf("ERROR: Invalid choice\n");
            return 1;
        }

        printf("Enter new value: ");
        scanf(" %[^\n]", new_value);

        editInfo.filename = argv[2];
        editInfo.new_data = new_value;

        switch (choice)
        {
            case 1:
                editInfo.tag = "-t";
                break;

            case 2:
                editInfo.tag = "-a";
                break;

            case 3:
                editInfo.tag = "-A";
                break;

            case 4:
                editInfo.tag = "-y";
                break;

            case 5:
                editInfo.tag = "-m";
                break;
        }

        edit_tag(&editInfo);
    }

    /* INVALID OPTION */
    else
    {
        printf("ERROR: Invalid option: %s\n", argv[1]);
        print_usage();
        return 1;
    }

    return 0;
}