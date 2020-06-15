/**
 * -----------------------------------------------------------------------------------------
 * @file        : Assembler/secondStage.c
 * @author      : Netanel Shoshan
 * @details     : secondStage and other parsing functions.
 * -----------------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "system.h"
#include "stages.h"
#include "utils.h"

/* output arrays */
static char outArr1[OUTPUT_ARR_LENGTH + 1], outArr2[OUTPUT_ARR_LENGTH + 1], *col1, *col2;

/* lineCnt is calculated with IC_OFFSET in order to determine the line */
static int lineCnt;

/**
 * This function is responsible for:
 * - parse unfinished lines.
 * - writing output files  (and deleting them if errorFlag=1)
 * @param file  {CLO ptr}
 * @param numOfLines {int}
 * @param filePath {CH ptr}
 * @return 0
 */
void secondStage(File_input *file, int numOfLines, char *filePath) {
    char fileName[MAX_FILENAME];
    lineCnt = 0;

    /* Create the files with the the proper extension, and in the org path. */
    sprintf(fileName, "%s.ob", filePath);
    ob = fopen(fileName, "w+");
    sprintf(fileName, "%s.ent", filePath);
    ent = fopen(fileName, "w+");
    sprintf(fileName, "%s.ext", filePath);
    ext = fopen(fileName, "w+");

    writeOb(file, numOfLines);      /* write .ob output file */
    writeEnt();                     /* write .ent output file */
    clearTables();                  /*clear the tables for further use */

    /*Close files */
    fclose(ob);
    fclose(ext);
    fclose(ent);

    /**
    * errorFlag and unnecessary files validations.
    * errorFlag - errors occurred during first/second stage
    * if externCnt or entryCnt equals 0 - means no label declaration been made
    * so will delete based on counter value.
    */
    switch (errorFlag) {
        case 1:
            sprintf(fileName, "%s.ob", filePath);
            remove(fileName);
            sprintf(fileName, "%s.ext", filePath);
            remove(fileName);
            sprintf(fileName, "%s.ent", filePath);
            remove(fileName);
            break;
        case 0:
            if (externCnt == 0) {
                sprintf(fileName, "%s.ext", filePath);
                remove(fileName);
                break;
            }
            else if (entryCnt == 0) {
                sprintf(fileName, "%s.ent", filePath);
                remove(fileName);
                break;
            }
            break;
    }
}


/**
 * Writing entry objects from entLabelArary into .ent output file.
 * Will search for the label in both dataSymTable and instTable.
 */
void writeEnt() {
    for (i = 0; entryArray[i]; i++) {

        /* If label was found in the dataSymTable*/
        if ((tmpNode = lookup(entryArray[i], dataSymTable))) {
            col1 = entryArray[i];
            col2 = baseConvertor(tmpNode->val + IC_OFFSET + 1 + ic, DEC_BASE, outArr1, PAD7);
            fprintf(ent, "%s\t%s\n", col1, col2);
        }
            /*The label is in the instTable */
        else if ((tmpNode = lookup(entryArray[i], instTable))) {
            col1 = entryArray[i];
            col2 = baseConvertor(tmpNode->val + IC_OFFSET + 1, DEC_BASE, outArr1, PAD7);
            fprintf(ent, "%s\t%s\n", col1, col2);
        }
            /* entry wasn't found on both instTable and dataSymTable */
        else {
            error(RED"*writeEnt: "RST"can't find address for entry %s", entryArray[i]);
            errorFlag = 1;
        }
    }
}

/**
 * table cleaning function for further use.
 */
void clearTables() {
    for (i = 0; i < HASHSIZE; i++) {
        dataSymTable[i] = NULL;
        instTable[i] = NULL;
        externalTable[i] = NULL;
    }
}

/**
 * Function to write .ob output file.
 * the file will run through the file after first stage.
 * In each iteration, based on isDone flag (that indicates that a line need parsing)
 * the function will call another assisting function called morePNedded (which also write to files).
 * @param file {ptr}
 * @param numOfLines number of lines in the input file.
 */
void writeOb(File_input *file, int numOfLines) {
    int j;

    /*object file header*/
    col1 = baseConvertor(ic, DEC_BASE, outArr1, ZPAD);
    col2 = baseConvertor(dc, DEC_BASE, outArr2, ZPAD);
    fprintf(ob, "     %s %s\n", col1, col2);
    lineCnt++;

    for (i = 0; i < numOfLines; i++) {
        /* If the line needs more parsing.*/
        if (!file[i].isDone) {

            /*validate addressing methods for the instruction opcode */
            addressChecking(file[i].instruction, file[i].lineNum);

            /* parse the line based on the number of repeated operand (DOC WAS GIVEN IN instructionHandler) */
            for (j = 0; j < file[i].moreParsing; j++) {

                /* write whole line the the ob file */
                col1 = baseConvertor(lineCnt + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int) convert_IL_to_data(*(file[i].instruction)).line, HEX_BASE, outArr2, PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);
                lineCnt++;

                /* If the source operand needs more parsing */
                if (file[i].srcOpr) {
                    morePNedded(file[i].srcOpr, (char) file[i].instruction->srcAdd, ob, ext, &lineCnt,
                                file[i].lineNum);
                }

                /* If the destination operand need more parsing */
                if (file[i].dstOpr) {
                    morePNedded(file[i].dstOpr, (char) file[i].instruction->dstAdd, ob, ext, &lineCnt,
                                file[i].lineNum);
                }
            }
        }
    }


    /**  writing line objects from dataArary into the ob file.
     *  line will be written to specific lines and not random ones. */
    for (i = 0; i < dc; i++) {
        col1 = baseConvertor(lineCnt + IC_OFFSET, DEC_BASE, outArr1, PAD7);
        col2 = baseConvertor((int) dataArray[i].line, HEX_BASE, outArr2, PAD6);
        fprintf(ob, "%s\t%s\n", col1, col2);
        lineCnt++;
    }
}

/**
 * Assisting function for secondStage.
 * will parse operands that needs more parsing.
 * @param strPtr - the operand
 * @param address - addressing method
 * @param ob {file ptr}
 * @param ext {file ptr}
 * @param ent {file ptr}
 * @param icLineCnt {int ptr}
 * @param lineNum {int}
 */
void morePNedded(char *strPtr, char address, FILE *ob, FILE *ext, int *icLineCnt, int lineNum) {
    /*vars decelerations*/
    int sign = 1, sum = 0;

    /* Addressing method */
    switch (address) {
        case 0:
            /*Instant addressing.
                 * handles both positive and negative numbers, covert it to 24 bit line line using the numberToData func.*/
            strPtr++;
            if (strPtr[0] == '-') {
                sign = -1;
                strPtr++;
            }
            while (isdigit(strPtr[0])) {
                sum *= 10;
                sum += strPtr[0] - '0';
                strPtr++;
            }

            col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            col2 = baseConvertor((int) numberToData(sum * sign, ABSOLUTE).line, HEX_BASE, outArr2, PAD6);
            fprintf(ob, "%s\t%s\t\n", col1, col2);
            (*icLineCnt)++;
            break;
        case 1:
            /*Immidiate addressing.
                search for the operand in the line,inst,external tables and write output to the files based on the rules.*/
            if ((tmpNode = lookup(strPtr, dataSymTable))) {
                col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int) numberToData(tmpNode->val + ic + IC_OFFSET + 1, RELOCATABLE).line,
                                     HEX_BASE,
                                     outArr2,
                                     PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);
                (*icLineCnt)++;
            }
            else if ((tmpNode = lookup(strPtr, instTable))) {
                col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int) numberToData(tmpNode->val + IC_OFFSET + 1, RELOCATABLE).line,
                                     HEX_BASE,
                                     outArr2,
                                     PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);
                (*icLineCnt)++;
            }
            else if ((tmpNode = lookup(strPtr, externalTable))) {

                col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int) numberToData(0, EXTERNAL).line, HEX_BASE, outArr2,
                                     PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);

                col1 = strPtr;
                col2 = baseConvertor(*icLineCnt + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                fprintf(ext, "%s\t%s\n", col1, col2);
                (*icLineCnt)++;
            }
            else {
                error(RED"*morePNedded: "RST"Can't find "YEL"\"%s\""RST" in ext | inst | data tables.\t| in line %d",
                      strPtr, lineNum);
            }
            break;
        case 2:
            /* Relative addressing.
                 * make sure the operand starts with '&' and put out an error if needed.
                 * search for the label in inst/dataSym tables and output the line as needed.
                 * if after all */
            if (strPtr[0] != '&') {
                error(RED"*morePNedded: "RST"Relative addressing format must start with &.\t| in line %d",
                      lineNum);
                break;
            }
            strPtr++;
            if ((tmpNode = lookup(strPtr, instTable))) {
                col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int) numberToData(tmpNode->val - *icLineCnt + 2, ABSOLUTE).line, HEX_BASE,
                                     outArr2,
                                     PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);
                (*icLineCnt)++;
            }
            else if ((tmpNode = lookup(strPtr, dataSymTable))) {
                col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int) numberToData(tmpNode->val + ic + IC_OFFSET + 1, ABSOLUTE).line,
                                     HEX_BASE,
                                     outArr2,
                                     PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);
                (*icLineCnt)++;
            }
            else {
                error(RED"*morePNedded: "RST"label %s dosen't supported for relative addressing.\t| in line %d", strPtr,
                      lineNum);
                break;
            }
    }
}

/**
 * Addressing method validation.
 * The validation is being made by opcodes.
 * @param instLine {ptr}
 * @param lineNum {int}
 */
void addressChecking(Instruction *instLine, int lineNum) {
    switch ((unsigned int) instLine->opCode) {
        case 0:
            if ((unsigned int) instLine->srcAdd == 2)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if (((unsigned int) instLine->dstAdd) == 2 || ((unsigned int) instLine->dstAdd) == 0)
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 1:
            if ((unsigned int) instLine->srcAdd == 2 || (unsigned int) instLine->dstAdd == 2)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            break;
        case 2:
            if ((unsigned int) instLine->srcAdd == 2)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if (((unsigned int) instLine->dstAdd) == 2 || ((unsigned int) instLine->dstAdd) == 0)
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 4:
            if ((unsigned int) instLine->srcAdd != 1)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if (((unsigned int) instLine->dstAdd == 0) || ((unsigned int) instLine->dstAdd == 2))
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 5:
            if ((unsigned int) instLine->srcAdd != 0)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if (((unsigned int) instLine->dstAdd == 0) || ((unsigned int) instLine->dstAdd == 2))
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 9:
            if ((unsigned int) instLine->srcAdd != 0)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if ((unsigned int) instLine->dstAdd == 0 || (unsigned int) instLine->dstAdd == 3)
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 12:
            if ((unsigned int) instLine->srcAdd != 0)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if ((unsigned int) instLine->dstAdd == 2 || (unsigned int) instLine->dstAdd == 0)
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 13:
            if ((unsigned int) instLine->srcAdd != 0)
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            else {
                if ((unsigned int) instLine->dstAdd == 2)
                    error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            }
            break;
        case 14:
            if (((unsigned int) instLine->srcAdd != 0) || ((unsigned int) instLine->dstAdd != 0))
                error(RED"*addressChecking:"RST"Illegal addressing mode.\t|in line %d", lineNum);
            break;
    }
}


