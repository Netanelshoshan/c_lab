/**
 * @ Netanel Shoshan @
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
 * then, the function will invoke fi and second stage functions.
 */
int main(int argc, char **argv) {
    /* var deceleration */
    int i;
    FILE *fp;
    char fileName[MAX_FILENAME], *tmpF;
    
    /* print welcome + help massages */
    if (argc == 1) {
        printf( "\n%s\n" , LINE);
        printf("\t  %s | %s\n", PROG_NAME, VERSION);
        printf("%s\n", HELP);
        printf( "%s\n\n" , LINE);
    }
    
    if (argc > 1) {
        /* print welcome message */
        printf( "\n%s\n" , LINE);
        printf("\t  %s | %s\n", PROG_NAME, VERSION);
        printf( "%s\n\n" , LINE);
    }
    
    initOpCodes(); /* opcodes initializer */
    
    for (i = 1; i < argc; i++) {
        int lineCnt = 0, bufCnt = BUF_SIZE; /*line and buffer counters*/
        int loffset = 0; /* for offsets between new lines \ comments vs. orginal lines*/
        File_input *file, *tmp; /* File_input pointers */
        tmpF = argv[i];
        
        /* max file length validation*/
        if (strlen(argv[i]) > MAX_FILENAME)
        {
            printf( "\n%s----\n" , LINE);
            error( "*ERROR: "  "File name can be up to 20 characters.");
            printf( "%s----\n" , LINE);
            exit(2);
        }
        
        /* append .as extension for the input file and open it*/
        sprintf(fileName, "%s.as", argv[i]);
        fp = fopen(fileName, "r");
        
        /* file was given with the .as extension*/
        if (fp == NULL) {
            printf( "\n%s----\n" , LINE);
            error( "*ERROR: "  "Please provide \"%s\" without the "  ".as "  "extension.\n", tmpF);
            printf( "%s----\n" , LINE);
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
                tmp = realloc(file, sizeof(File_input)*bufCnt);
                tmp = realloc(file, sizeof(File_input) * bufCnt);
                if (tmp)
                    file = tmp;
                else { /* cant allocate more space */
                    error( "*SYSTEM: buf is full and mem realloc - FAILED." );
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
        firstStage(file, lineCnt);           /* initialize fi stage */
        secondStage(file, lineCnt, argv[i]); /* initialize fi stage */
        free(file);                         /* free File_input object for next iteration */
        fclose(fp);                         /* close the current file */
        status(fileName, errorFlag, 0);     /* put out a status message for given file */
    }
    return 0;
}
