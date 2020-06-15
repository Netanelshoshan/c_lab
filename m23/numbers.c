/*
 * =========================================================================================
 * name        : mmn_23/numbers.c
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : program to convert given numbers into words.
 * =========================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



/* Global and prototype decelerations */
#define PROG_NAME      "NUMBERS"
#define VERSION        "MMN_23 v1.0 316393321"
#define LINE           "============================================================="
#define MAXBUFFER 300
#define HELP           "Usage:\tnumbers [number]\n\tnumbers [input_file]\n\tnumbers [input_file] [output_file]"
#define HALT           "*INFORM: Program halted.\n"

void numnum(char *num);

/* word-Array definitions */
char *singleDigit[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
char *tensDigit[] = {"", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
                     "eighteen",
                     "nineteen"};
char *tensMultiple[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};


/*
 * Gets an string array containing integers and converts
 * them into words.*/
void numnum(char *num) {
    int len = strlen(num);

    /*empry string*/
    if (len == 0 || len > 2) {
        fprintf(stderr, "*ERROR: Invalid number - %s.\n*INFORM: Please provide numbers within the range 0-99.\n", num);
        printf("%s", HALT);
        exit(1);
    }

    /*single digit case*/
    if (len == 1) {
        printf("%s\n", singleDigit[*num - '0']);
        return;
    }
    /*two digit case*/
    while (*num != '\0') {

        /*the sum of the two digit will be the index of the string at tens array.*/
        if (*num == '1') {/*10-19 cases*/
            int sum = *num - '0' + *(num + 1) - '0';
            printf("%s\n", tensDigit[sum]);
            break;
        } else if (*(num + 1) == '0') {/* ten'th digit change occurred*/
            int tens_change = *num - '0';
            printf("%s\n", tensMultiple[tens_change]);
            break;
        }
            /* The algorithm is:
             * get the digit at num[0].
             * is it in the array containing number twenty-ninety ?
             * if it is , print it and move the pointer to next pos.
             * if num["rest"] is not 0, print the string at that index.
            */
        else {
            int i = *num - '0';
            printf("%s ", tensMultiple[i]);
            ++num;
            if (*num != '0') {
                printf("%s ", singleDigit[*num - '0']);
                printf("\n");
            }
        }
        ++num;
    }
}/*end of numnum */

/*main function*/
int main(int argc, char **argv) {

    /*variable decelerations*/
    FILE *in_file;
    FILE *out_file;
    char file_buffer[MAXBUFFER];

    /*welcome message*/
    printf("%s\n", LINE);
    printf("\t\t%s, %s\n", PROG_NAME, VERSION);
    printf("%s\n", LINE);

    /*more than one command(or a file) was given */
    if (argc > 3) {
        fprintf(stderr, "*INFORM: You cant provide more than two files.\n");
        exit(1);
    }

    /*no files was given*/
    if (argc == 1) {
        printf("%s\n%s\n", HELP, LINE);
        exit(0);
    }
    /*file was given*/
    if (argc == 2) {

        if (!isalpha(*argv[1])) {
            numnum(argv[1]);
            exit(0);
        }

        in_file = fopen(argv[1], "r");/*open with read perm*/
        if (in_file == NULL) { /* handles empty file */
            fprintf(stderr, "*ERROR: An error occurred with opening %s\n", argv[1]);
            exit(1);
        }

        /*run through the the file and send the data to numnum()*/
        while (fscanf(in_file, "%s", file_buffer) != EOF) {
            numnum(file_buffer);
        }

        fclose(in_file);/*close it*/
        exit(0);/*exit*/
    }
    if (argc == 3) {
        /*opening the files with read permissions*/
        in_file = fopen(argv[1], "r");
        out_file = fopen(argv[2], "r+");

        if (in_file == NULL) { /* handles empty file */
            fprintf(stderr, "*ERROR: An error occurred with opening %s or %s\n", argv[1], argv[2]);
            exit(1);
        }/*runs through the the file and send the data to numnum()*/

        /* output file overriding*/
        if (out_file != NULL) {
            char res;

            printf("*INFORM: file already exist. override it - [Y] / [N] ? ");
            system("stty raw");
            while (1) {
                scanf("%c", &res);
                if (tolower(res) == 'y') {
                    system("stty cooked");
                    printf("\n*INFORM: file \"%s\" was overrided.\n", argv[2]);
                    freopen(argv[2], "w+", stdout); /*redirecting stdout and writing it to the given op file */
                    while (fscanf(in_file, "%s", file_buffer) != EOF) {
                        numnum(file_buffer);
                    }
                    fclose(in_file);
                    fclose(out_file);
                    exit(0);
                } else {
                    system("stty cooked");
                    printf("\n%s", HALT);
                    exit(0);
                }
            }
        }/*end of output file overriding*/
        out_file = fopen(argv[2], "w+");
        /* Creating output file in case there isn't one..*/
        printf("*INFORM: Output file \"%s\" was created successfully.\n", argv[2]);
        freopen(argv[2], "w+", stdout); /*redirecting stdout and writing it to given op file */
        while (fscanf(in_file, "%s", file_buffer) != EOF) {
            numnum(file_buffer);
        }

        /*close the files and exit*/
        fclose(out_file);
        fclose(in_file);
        exit(0);/*exit*/

    }/*end of argc 3*/
    return 0;
}/*end of main*/
