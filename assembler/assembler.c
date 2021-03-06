/**
 * @Netanel Shoshan@
 * 
 * Contains the main func for the assembler program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "system.h"
#include "stages.h"

/*
 * The function gets the num of arguments and the list of files from the command line.
 * The fucntion will initialize, count and allocate enough space for each line in the given file.
 * then, the function will invoke first and second stage functions.
 */
int main(int argc, char **argv) {
    /* var deceleration */
    int i;
    FILE *fp;
    char fileName[MAX_FILENAME], *tmpF;

    /* print welcome + help massages */
    if (argc == 1) {
        printf(GRN "\n%s\n" RST, LINE);
        printf("\t  %s | %s\n", PROG_NAME, VERSION);
        printf("%s\n", HELP);
        printf(GRN "%s\n\n" RST, LINE);
    }

    if (argc > 1) {
        /* print welcome message */
        printf(GRN "\n%s\n" RST, LINE);
        printf("\t  %s | %s\n", PROG_NAME, VERSION);
        printf(GRN "%s\n\n" RST, LINE);
    }

    initOpCodes(); /* opcodes initializer */

    for (i = 1; i < argc; i++) {
        int lineCnt = 0, bufCnt = BUF_SIZE; /*line and buffer counters*/
        int loffset = 0; /* for offsets between new lines \ comments vs. orginal lines*/
        File_input *file, *tmp; /* File_input pointers */
        tmpF = argv[i];

        /* max file length validation*/
        if (strlen(argv[i]) > MAX_FILENAME) {
            printf(YEL "\n%s----\n" RST, LINE);
            error(YEL "*ERROR: " RST "File name can be up to 20 characters.");
            printf(YEL "%s----\n" RST, LINE);
            exit(2);
        }

        /* append .as extension for the input file and open it*/
        sprintf(fileName, "%s.as", argv[i]);
        fp = fopen(fileName, "r");

        /* file was given with the .as extension*/
        if (fp == NULL) {
            printf(YEL "\n%s----\n" RST, LINE);
            error(YEL "*ERROR: " RST "Please provide \"%s\" without the " RED ".as " RST "extension.\n", tmpF);
            printf(YEL "%s----\n" RST, LINE);
            exit(1);
        }

        /* memory allocation for file reading */
        file = malloc(sizeof(File_input) * BUF_SIZE);
        file[lineCnt].line.content = malloc(sizeof(char) * LINE_LENGTH);

        /* read the file */
        while (fgets(file[lineCnt].line.content, LINE_LENGTH, fp)) {

            /* new lines and comment indicators */
            if (isNewLine(&(file[lineCnt]).line.content) || isComment(&(file[lineCnt]).line.content)) {
                loffset++;
                continue;
            }

            /* if there's a need for memory reallocation */
            if (lineCnt == bufCnt) {
                bufCnt += BUF_SIZE;
                tmp = realloc(file, sizeof(File_input) * bufCnt);
                if (tmp)
                    file = tmp;
                else { /* cant allocate more space */
                    error(RED "*SYSTEM: buf is full and mem realloc - FAILED." RST);
                    exit(2);
                }
            }

            /* increment the line counter */
            file[lineCnt].lineNum = lineCnt + 1 + loffset;
            
            /* for each line, allocate enough memory as required (80 chars) */
            file[++lineCnt].line.content = malloc(sizeof(char) * LINE_LENGTH);
        }
        free(file[lineCnt].line.content);    /* free the last '\n' */
        status(tmpF, 0, 1);                  /*put out starting status for the file*/
        firstStage(file, lineCnt);           /* initialize first stage */
        secondStage(file, lineCnt, argv[i]); /* initialize first stage */
        free(file);                         /* free File_input object for next iteration */
        fclose(fp);                         /* close the current file */
        status(fileName, errorFlag, 0);     /* put out a status message for given file */
    }
    return 0;
}
