/**
 * @ Netanel Shoshan @
 *
 * Assisting functions such as : base-conversion, space trimming, new line skipping, etc..
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "utils.h"
#include "system.h"

/* swap function that is used in num conversion */
#define SWAP(x, y)            \
    do                        \
    {                         \
        __typeof__(x) _x = x; \
        __typeof__(y) _y = y; \
        x = _y;               \
        y = _x;               \
    } while (0)

/* A utility function to reverse a string */
void reverse(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        SWAP(*(str + start), *(str + end));
        start++;
        end--;
    }
}

/* A utility function to convert any number to desi base.*/
char *baseConvertor(int num, int base, char *str, int pad)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitly, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        str = padWithZees(str, pad);
        return str;
    }

    /* Negative numbers are handled only with
    base 10. Otherwise numbers are conside unsigned.*/
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    /*Process individual digits*/
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    /* If number is negative, append '-'*/
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; /* Append string terminator*/

    /* Reverse the string*/
    reverse(str, i);
    if (pad)
        str = padWithZees(str, pad);
    return str;
}

/* Trimming the fi spaces in the array */
void spaceTrimmer(char **str)
{
    while (isspace(**str) && (**str != '\n'))
        (*str)++;
}

/* New line indicator */
int isNewLine(char **str)
{
    return ((**str) == '\n') ? 1 : 0;
}

/* Comment indicator */
int isComment(char **str)
{
    return ((**str) == ';') ? 1 : 0;
}

/* make duplicate of s */
char *makeDup(const char *s)
{
    char *dup;
    dup = (char *)malloc(strlen(s) + 1);
    if (dup != NULL)
        strcpy(dup, s);
    return dup;
}

/* Function for pad a string with desi zeros */
char *padWithZees(char *s, int padLen)
{
    char *tmp = malloc(padLen + 1);
    int i, j = 0;
    for (i = 0; i < padLen; i++)
        tmp[i] = '0';
    i = (int)strlen(s);
    while (i > 0)
    {
        tmp[padLen - 1 - j] = s[strlen(s) - 1 - j];
        i--;
        j++;
    }
    return tmp;
}

/**
 * Status indicator.
 * The function will put out a message when the interpretation for a certin file has begun.
 * by the end of each file the function will be invoked again and put out if the interpretation
 * was successful of not.
 */
void status(char *fileName, int eflag, int isStart)
{
    switch (isStart)
    {
    case 1:

        printf( "\n%s\n" , LINE);
        printf( "*SYSTEM: "  "Starting interpretation for \"%s\"\n", fileName);
        printf( "%s\n\n" , LINE);
        break;

    default:
        if (eflag == 0)
        {
            printf( "\n%s\n" , LINE);
            printf( "*SYSTEM: "  "Interpretation Status for \"%s\" - "  "SUCCESS.\n" , fileName);
            printf( "%s\n\n" , LINE);
            break;
        }
        else
        {
            printf("\n"  "%s------------------------\n" , LINE);
            printf( "*SYSTEM: "  "Due to error occur while interpreting \"%s\". Status - "  "FAILED.\n" ,
                   fileName);
            printf( " TOTAL ERRORS : "  "%d\n", totalErr);
            printf(""  "%s------------------------\n" , LINE);
            break;
        }
    }
}

/**
 * Errors handling function.
 * The function has build in template and adds the string to
 * the template when invoked. (also print in which line the error occu.)
 * */
void error(char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    fprintf(stderr,  "*error: " );
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    va_end(args);
    totalErr++;    /*total errors counter */
    errorFlag = 1; /* sets error flag value to 1 */
}
