/**
 * @ Netanel Shoshan @
 *
 * Structures definitions and function prototypes for table mgmt.
 */
#ifndef STRUCTS_H
#define STRUCTS_H

#include "system.h"

/* 24 bit data structure */
typedef struct dl
{
    unsigned int line : BIT_WIDTH; /* 24 bit line*/
} data;

/* Instruction line structure */
typedef struct instructionLine
{
    /* each cell gets his own width */
    unsigned int E : TYPE_WIDTH;
    unsigned int R : TYPE_WIDTH;
    unsigned int A : TYPE_WIDTH;
    unsigned int funct : FUNCT_WIDTH;
    unsigned int dstReg : DREG_WIDTH;
    unsigned int dstAdd : DADDR_WIDTH;
    unsigned int srcReg : SREG_WIDTH;
    unsigned int srcAdd : SADDR_WIDTH;
    unsigned int opCode : OPCODE_WIDTH;
} Instruction;

/* Code line structure for file parsing */
typedef struct file_input
{
    struct
    {
        char *content;   /* line content */
        int moreParsing; /* if the line need's more than one parsing (used in 2nd stage) - (for labels) */
        int isDone;      /* marking line as done only if the line doesn't require more the one parsing. - (for whole line) */
    } line;
    Instruction *instruction; /* each line hold a ptr to "instructionLine" object */
    int lineNum;              /* line number for error handling and more */
    char *srcOpr;             /* if src op is a label, it will hold it, otherwise it's a reg and "instructionLine" will hold it */
    char *dstOpr;             /* ^same goes here^ */
} File_input;

/* structure that can hold opcodes and being used for other purposes like labels and symbols*/
typedef struct sysNode
{
    struct sysNode *next;
    char *name;
    int val;
    int funct;
} sysNode;

sysNode *opcodeTable[HASHSIZE];      /* opcode Array*/
sysNode *dataSymbolsTable[HASHSIZE]; /* array for 24 bit line data symbols (STR,etc..) */
sysNode *instTable[HASHSIZE];        /* labels array (LOOP,MAIN,etc..) that instruction comes after them*/
sysNode *extTable[HASHSIZE];         /* external labels array */
sysNode *tmpNode;                    /* tmp node for changes on the fly.*/
data dataArray[ARR_SIZE];            /* 24 bit dataArray */
char *entryTable[ARR_SIZE];          /* entry labels array */

/* ------------------- table management function prototypes ---------------------*/

sysNode *lookup(char *string, sysNode **table);                             /* install's helper. will return null of the string wasn't found */
sysNode *install(char *name, int val, sysNode **table);                     /* for installations purposes */
sysNode *installOpcode(char *name, int funct, int opcVal, sysNode **table); /* responsible for opcode init */
data charToData(char ch);                                                   /* character to 24 bit line converter */
data numberToData(int num, int are);                                        /* converting a singed int to 24 bit line */
data convert_IL_to_data(Instruction instLine);                              /* instruction to 24 bit instLine converter */
unsigned int hash(char *s);                                                 /* hashing function */
int functFetcher(char *string, sysNode **table);                            /* funct fetcher */
void initOpCodes();                                                         /* opcode table initializer */
void initInstructionLine(Instruction *instLine);                            /* instruction line initializer */
void initDataLine(data *dataPtr);                                           /* data line initializer */

#endif
