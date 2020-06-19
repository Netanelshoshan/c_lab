/*
 * @ Netanel Shoshan @
 * 
 * Global vars and structure macros for system use.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

/*-------------- system vars and macros ----------------*/

/* global vars for system use */
int entryCnt, externCnt;
int i, ic, dc;
int errorFlag, totalErr;

/* Color indicators */
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define RST "\x1B[0m"

/* Warm welcome */
#define PROG_NAME "ASSEMBLER"
#define VERSION "MMN_14 v1.3 | 316393321"
#define LINE "----------------------------------------------------------"
#define HELP "Usage:\tassembler" YEL " [input_file]" RST GRN

/* Default macros */
#define MAX_FILENAME 20
#define LINE_LENGTH 80
#define BUF_SIZE 100
#define ARR_SIZE 100 /* same as buff size so there will be no conflicts later */

/* for writing output files */
#define OUTPUT_ARR_LENGTH 7
#define IC_OFFSET 99

/* for base conversion purposes */
#define HEX_BASE 16
#define DEC_BASE 10
#define ZPAD 0
#define PAD6 6
#define PAD7 7

#define HASHSIZE 200 /* used by install when hashing a string */

/* -------------- structure macros-------------------*/

#define BIT_WIDTH 24 /* line width */

/* Width of each cell in data line */
#define TYPE_WIDTH 1
#define FUNCT_WIDTH 5
#define DREG_WIDTH 3
#define DADDR_WIDTH 2
#define SREG_WIDTH 3
#define SADDR_WIDTH 2
#define OPCODE_WIDTH 6

/* data line offsets  */
#define OPCODE_OFFSET 18
#define SADDR_OFFSET 16
#define SREG_OFFSET 13
#define DADDR_OFFSET 11
#define DREG_OFFSET 8
#define FUNCT_OFFSET 3
#define ABSOLUTE_OFFSET 2
#define RELOCATABLE_OFFSET 1
#define EXTERNAL_OFFSET 0

/* for A,R,E handling*/
enum
{
    EXTERNAL,
    RELOCATABLE,
    ABSOLUTE,
    X
};

#endif
