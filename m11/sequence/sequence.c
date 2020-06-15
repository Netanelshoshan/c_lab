/*
 * =====================================================================================================
 * name        : mmn_11/sequence/sequence.c
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : f_sequence function gets a string, determine if the string is in
 * constant ascending / decending order or not at all.
 * =====================================================================================================
 */

#include "common.h"

/* This function determine if the string is in constant ascending / descending ot not constant order.*/
void f_sequence(char s[])
{

    int down = 0, up = 0, dups = 0; /* Initializing counters */
    int len = strlen(s);            /* Getting the length of the string using strlen() func. */
    int i;

    /*Measuring each character with his next one and changing the counters if needed */
    for (i = 0; i < len - 1; i++)
    {
        if (s[i] < s[i + 1])
            up++;
        else if (s[i] > s[i + 1])
            down++;
        else if (s[i] == s[i + 1])
            dups++;
    }

    /*Output messages based on counter values */
    if (up > 0 && down == 0 && dups == 0)
        printf("The string \"%s\" is in constant ascending order.\n", s);

    else if (up > 0 && down == 0 && dups > 0)
        printf("The string \"%s\" is in ascending order.\n", s);

    else if (up == 0 && down > 0 && dups == 0)
        printf("The string \"%s\" is in constant descending order.\n", s);

    else if (up == 0 && down > 0 && dups > 0)
        printf("The string \"%s\" is in descending order.\n", s);

    else if (up > 0 && down > 0 && dups > 0)
        printf("The string \"%s\" is NOT in constant order.\n", s);

    else
        printf("The string \"%s\" is in ascending order.\n", s);
}

/* Main func to operating the program */
int main(int argc, char **argv)
{
    /*Variable decelerations*/
    FILE *sFile;
    char s[MAXCHAR];
    char sBuffer[MAXFILE];

    /* If one argument was given */
    if (argc == 1)
    {
        fprintf(stderr, "\nNo files were given.\n");
        fprintf(stderr, "For input as a file: ./sequence <strings file>\n");
        printf("\nPlease provide the string: ");
        scanf("%s", s);
        printf("The string provided is \"%s\"\n", s);
        printf("\n");
        f_sequence(s);
        exit(0);
    }

    /* If two arguments was given */
    if (argc == 2)
    {
        sFile = fopen(argv[1], "r"); /* opens the file with read permissions*/
        if (!sFile)                  /* can't open the file */
        {
            fprintf(stderr, "An error occurred with opening %s \n", argv[1]);
            exit(2);
        }
        while (fscanf(sFile, "%s", sBuffer) != EOF)
            f_sequence(sBuffer);
        fclose(sFile);
        exit(3);
    }
    return 0;
}
