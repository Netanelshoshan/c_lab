/*
 * =====================================================================================================
 * name        : mmn_11/match/match.c
 * author      : Netanel Shoshan ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : strMatch function gets two strings and return the index with
 * max approximate strings match.
 * =====================================================================================================
 */

#include "common.h"
/*
 * This function will perform approximate string matching and will
 * return the starting index were the string is with most matching characters.
 */
int strMatch(char *pattern, char *string) {
    /* Variable decelerations */
    int strLen = strlen(string);
    int patLen = strlen(pattern);
    int open_index = -1;
    int i = 0, j = 0;
    int matchLocation = 0;
    int strPtr = 0;
    int patPtr = 0;
    char stringToCheck[MAXCHAR];
    char prevString[MAXCHAR];

    /*If pattern length is greater than the string length*/
    if (strLen < patLen)
        return -1;

    while (j < strLen) {
        /* Returns the location of char "c" in "string", starting from index "j" */
        matchLocation = charFinder(string, pattern[i], j);

        /* The character wasn't found */
        if (matchLocation == -1)
            break;

        /*The char was found but not in ideal location */
        if (patLen != 1 && matchLocation == strLen - 1)
            break;

        /* Initializing pointers for the string and the pattern */
        strPtr = 0;
        patPtr = 0;

        /* Creating a temp string starting from where matchLocation pointing to.
         * (same length as the pattern) */
        for (strPtr = matchLocation; strPtr < strLen && patPtr < patLen; patPtr++, strPtr++)
            stringToCheck[patPtr] = string[strPtr];

        /*
	 * i=0 means that the we currently don't have previous string to compare to,
         * so will assign the current string as the "best option".
         * Later on (else section), will compare two strings to the pattern
         * and will operate based on the string with the lowest "errors". */
        if (i == 0) {
            cp_arr(stringToCheck, prevString);
            open_index = matchLocation;
            j++;
            i++;
        } else {
            if (my_strcmp(stringToCheck, pattern) < my_strcmp(prevString, pattern)) {
                cp_arr(stringToCheck, prevString);
                open_index = matchLocation;
                j++;
                i++;
            } else
                i++;
        }
    }
    return open_index;
}

/* function for array copy.*/
void cp_arr(char src[], char dst[]) {
    int i;

    /* Empty the destination array first, than copy everything.*/
    for (i = 0; i < strlen(dst); i++) {
        dst[i] = '\0';
    }

    for (i = 0; i < strlen(src); i++) {
        dst[i] = src[i];
    }
}

/* function for counting unmatched chars*/
int my_strcmp(char src[], char dst[]) {
    int cnt = 0, i;

    for (i = 0; i < strlen(dst); i++) {
        if (src[i] != dst[i])
            cnt++;
    }
    return cnt;
}

/* function for finding the location of c in s, starting from index k */
int charFinder(char *s, char c, int k) {
    int i;
    for (i = k; i < strlen(s); i++) {
        if (s[i] == c)
            return i;
    }
    return -1;
}

/* Main func for operating the program */
int main(int argc, char **argv) {
    
    /*Variable decelerations*/
    FILE *pFile;
    FILE *sFile;
    int res;
    char s[MAXCHAR];
    char sBuffer[MAXFILE];
    char p[MAXCHAR];
    char pBuffer[MAXFILE];

    /* If one argument was given */
    if (argc == 1) {
        fprintf(stderr, "No files were given.\n");
        fprintf(stderr, "For input as a file: ./match <patterns file> <strings file> \n");
        printf("\nPlease provide the pattern: ");
        scanf("%s", p);
        printf("Please provide the string: ");
        scanf("%s", s);
        printf("\nThe pattern provided is \"%s\"\n", p);
        printf("The string provided is \"%s\"\n", s);
        printf("\n");
        res = strMatch(p, s);
        printf("For \"%s\" as a pattern and \"%s\" as a string, the result is: %d\n", p,s,res);
        exit(0);
    }
    /* If two argument was given */
    if (argc == 2) {
        fprintf(stderr, "usage: ./match patterns strings\n");
        exit(1);
    }
    /* If three arguments was given */
    if (argc == 3) {
        pFile = fopen(argv[1], "r"); /* opens the file with read permissions*/
        sFile = fopen(argv[2], "r"); /* opens the file with read permissions*/
        if (pFile == NULL || sFile == NULL) /* can't open the file */
        {
            fprintf(stderr, "An error occurred with opening %s or %s\n", argv[1], argv[2]);
            exit(2);
        }
        while (fscanf(pFile, "%s", pBuffer) != EOF && fscanf(sFile, "%s", sBuffer) != EOF) {
            res = strMatch(pBuffer, sBuffer);
            printf("For \"%s\" as a pattern and \"%s\" as a string, the result is: %d\n", pBuffer, sBuffer,res);
        }
        fclose(pFile);
        fclose(sFile);
        exit(3);
    }
    return 0;
}

