/**
 * @ Netanel Shoshan @
 *
 * Function prototypes for first, second and main assembler functions.
 */
#ifndef STAGES_H
#define STAGES_H

#include "system.h"
#include "stages.h"
#include "structs.h"


/*--------- main assembler functions -------------*/

void firstStage(File_input *file, int numberOfLines);
void secondStage(File_input *file, int numOfLines, char *filePath);
FILE *ob, *ext, *ent;

/*-------------------- second stage functions ---------------------*/

void morePNedded(char *strPtr, char address, FILE *ob, FILE *ext, int *icLineCnt, int lineNum);
void addressChecking(Instruction *instLine, int lineNum);
void writeOb(File_input *file, int numOfLines);
void writeEnt();
void clearTables();

/*-------------------------- first stage functions --------------------------*/
void dataHandler(File_input *fInput);
void stringHandler(File_input *fInput);
void entryHandler(File_input *pInput);
void externHandler(File_input *pInput);
char expectedOperands(char *name);
void instructionHandler(File_input *fInput);
void commandHandler(File_input *fInput, char *symbol);
char *isLabel(File_input *fInput);

#endif
