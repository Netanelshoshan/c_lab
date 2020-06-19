/**
 * @Netanel Shoshan@
 * 
 * secondStage and other parsing functions.
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
 * This function will handle:
 * Lines that need more parsing.
 * Writing output files.
 */
void secondStage(File_input *file, int numOfLines, char *filePath)
{
    char fileName[MAX_FILENAME];
    lineCnt = 0; /* counter for the output lines */
    /*filePath = "/Users/mbp/i1";*/

    /* Create the files with the the proper extension, and in the org path. */
    sprintf(fileName, "%s.ob", filePath);
    ob = fopen(fileName, "w+");
    sprintf(fileName, "%s.ent", filePath);
    ent = fopen(fileName, "w+");
    sprintf(fileName, "%s.ext", filePath);
    ext = fopen(fileName, "w+");

    writeOb(file, numOfLines); /* write .ob output file */
    writeEnt();                /* write .ent output file */
    clearTables();             /*clear the tables for further use */

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
    switch (errorFlag)
    {
    case 1:
        sprintf(fileName, "%s.ob", filePath);
        remove(fileName);
        sprintf(fileName, "%s.ext", filePath);
        remove(fileName);
        sprintf(fileName, "%s.ent", filePath);
        remove(fileName);
        break;
    case 0:
        if (externCnt == 0)
        {
            sprintf(fileName, "%s.ext", filePath);
            remove(fileName);
            break;
        }
        else if (entryCnt == 0)
        {
            sprintf(fileName, "%s.ent", filePath);
            remove(fileName);
            break;
        }
        break;
    }
}

/**
 * Writing entry objects from entLabelArary into .ent output file.
 * Will search for the label in both dataSymbolsTable and instTable.
 */
void writeEnt()
{
    for (i = 0; entryTable[i]; i++)
    {
        /* If label was found in the dataSymbolsTable*/
        if ((tmpNode = lookup(entryTable[i], dataSymbolsTable)))
        {
            col1 = entryTable[i];
            col2 = baseConvertor(tmpNode->val + IC_OFFSET + 1 + ic, DEC_BASE, outArr1, PAD7);
            fprintf(ent, "%s\t%s\n", col1, col2);
        }
        /*The label is in the instTable */
        else if ((tmpNode = lookup(entryTable[i], instTable)))
        {
            col1 = entryTable[i];
            col2 = baseConvertor(tmpNode->val + IC_OFFSET + 1, DEC_BASE, outArr1, PAD7);
            fprintf(ent, "%s\t%s\n", col1, col2);
        }
        /* entry wasn't found on both instTable and dataSymbolsTable */
        else
            error("can't find address for entry" YEL " \"%s\"." RST, entryTable[i]);
    }
}

/**
 * table cleaning function for further use.
 */
void clearTables()
{
    for (i = 0; i < HASHSIZE; i++)
    {
        dataSymbolsTable[i] = NULL;
        instTable[i] = NULL;
        extTable[i] = NULL;
    }
}

/**
 * Function to write .ob output file.
 * when invoked, the function will run through the file.
 * In each iteration, based on isDone flag the fucntion will 
 * create a second loop and parse the src/dst oprand that needs more parsing.
 * (by calling morePNeeded)
 */
void writeOb(File_input *file, int numOfLines)
{
    int j;

    /*object file header*/
    col1 = baseConvertor(ic, DEC_BASE, outArr1, ZPAD);
    col2 = baseConvertor(dc, DEC_BASE, outArr2, ZPAD);
    fprintf(ob, "     %s %s\n", col1, col2);
    lineCnt++;

    for (i = 0; i < numOfLines; i++)
    {
        /* If the line needs more parsing.*/
        if (!file[i].line.isDone)
        {

            /*validate addressing methods for the instruction  */
            isValidAddr(file[i].instruction, file[i].lineNum);

            /* parse the line based on the number of repeated operand */
            for (j = 0; j < file[i].line.moreParsing; j++)
            {

                /* write whole line the the ob file */
                col1 = baseConvertor(lineCnt + IC_OFFSET, DEC_BASE, outArr1, PAD7);
                col2 = baseConvertor((int)instTo24B(*(file[i].instruction)).line, HEX_BASE, outArr2, PAD6);
                fprintf(ob, "%s\t%s\t\n", col1, col2);
                lineCnt++;

                /* If the source operand needs more parsing */
                if (file[i].srcOpr)
                    morePNedded(file[i].srcOpr, (char)file[i].instruction->srcAdd, ob, ext, &lineCnt,
                                file[i].lineNum);

                /* If the destination operand need more parsing */
                if (file[i].dstOpr)
                    morePNedded(file[i].dstOpr, (char)file[i].instruction->dstAdd, ob, ext, &lineCnt,
                                file[i].lineNum);
            }
        }
    }

    /**  writing line objects from dataArary into the ob file.
     *  line will be written to specific lines and not random ones. */
    for (i = 0; i < dc; i++)
    {
        col1 = baseConvertor(lineCnt + IC_OFFSET, DEC_BASE, outArr1, PAD7);
        col2 = baseConvertor((int)dataArray[i].line, HEX_BASE, outArr2, PAD6);
        fprintf(ob, "%s\t%s\n", col1, col2);
        lineCnt++;
    }
}

/**
 * Assisting function for secondStage.
 * will parse operands that needs more parsing.
 */
void morePNedded(char *strPtr, char address, FILE *ob, FILE *ext, int *icLineCnt, int lineNum)
{
    /*vars decelerations*/
    int sign = 1, sum = 0;

    /* addressing method validation */
    switch (address)
    {
    case 0:
        /*Instant addressing.
        * Handles both positive and negative numbers.
        * Covert it to 24 bit line line using the numberToData func and write the data.*/
        strPtr++;
        if (strPtr[0] == '-')
        {
            sign = -1;
            strPtr++;
        }
        while (isdigit(strPtr[0]))
        {
            sum *= 10;
            sum += strPtr[0] - '0';
            strPtr++;
        }

        col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
        col2 = baseConvertor((int)numTo24Bit(sum * sign, ABSOLUTE).line, HEX_BASE, outArr2, PAD6);
        fprintf(ob, "%s\t%s\t\n", col1, col2);
        (*icLineCnt)++;
        break;
    case 1:
        /*Immidiate addressing.
        * Search for the operand in the dataSymbols,inst,external 
        * tables and write output to the files based on the rules.*/
        if ((tmpNode = lookup(strPtr, dataSymbolsTable))) /* Label found in the dataSymbolsTable*/
        {
            col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            col2 = baseConvertor((int)numTo24Bit(tmpNode->val + ic + IC_OFFSET + 1, RELOCATABLE).line,
                                 HEX_BASE,
                                 outArr2,
                                 PAD6);
            fprintf(ob, "%s\t%s\t\n", col1, col2);
            (*icLineCnt)++;
        }
        else if ((tmpNode = lookup(strPtr, instTable))) /* Label found in instructions table*/
        {
            col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            col2 = baseConvertor((int)numTo24Bit(tmpNode->val + IC_OFFSET + 1, RELOCATABLE).line,
                                 HEX_BASE,
                                 outArr2,
                                 PAD6);
            fprintf(ob, "%s\t%s\t\n", col1, col2);
            (*icLineCnt)++;
        }
        else if ((tmpNode = lookup(strPtr, extTable))) /* Label found in external table*/
        {

            col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            col2 = baseConvertor((int)numTo24Bit(0, EXTERNAL).line, HEX_BASE, outArr2,
                                 PAD6);
            fprintf(ob, "%s\t%s\t\n", col1, col2);

            /* write the label to external file */
            col1 = strPtr;
            col2 = baseConvertor(*icLineCnt + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            fprintf(ext, "%s\t%s\n", col1, col2);
            (*icLineCnt)++;
        }
        else
            /* Label wasn't found. */
            error("Can't find " YEL "\"%s\"" RST " in ext | inst | dataSym tables.| in line %d",
                  strPtr, lineNum);

        break;
    case 2:
        /* Relative addressing.
        * make sure the operand starts with '&' and put out an error if needed.
        * search for the label in inst/dataSym tables and output the line as needed.
        * if after all */
        if (strPtr[0] != '&')
        {
            error("Relative addressing format must start with &.\t| in line %d",
                  lineNum);
            break;
        }
        strPtr++;                                  /* advance the pointer after & */
        if ((tmpNode = lookup(strPtr, instTable))) /* Label found in the external table*/
        {
            col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            col2 = baseConvertor((int)numTo24Bit(tmpNode->val - *icLineCnt + 2, ABSOLUTE).line, HEX_BASE,
                                 outArr2,
                                 PAD6);
            fprintf(ob, "%s\t%s\t\n", col1, col2);
            (*icLineCnt)++;
        }
        else if ((tmpNode = lookup(strPtr, dataSymbolsTable))) /* Label found in dataSymbolsTable */
        {
            col1 = baseConvertor((*icLineCnt) + IC_OFFSET, DEC_BASE, outArr1, PAD7);
            col2 = baseConvertor((int)numTo24Bit(tmpNode->val + ic + IC_OFFSET + 1, ABSOLUTE).line,
                                 HEX_BASE,
                                 outArr2,
                                 PAD6);
            fprintf(ob, "%s\t%s\t\n", col1, col2);
            (*icLineCnt)++;
        }
        else
            /*Label wasn't found */
            error("label %s dosen't supported for relative addressing.\t| in line %d", strPtr,
                  lineNum);
        break;
    }
}

/**
 * Addressing method validation.
 * The validation is being made by opcodes.
 */
void isValidAddr(Instruction *instLine, int lineNum)
{
    switch ((unsigned int)instLine->opCode)
    {
    case 0:
        if ((unsigned int)instLine->srcAdd == 2)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if (((unsigned int)instLine->dstAdd) == 2 || ((unsigned int)instLine->dstAdd) == 0)
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 1:
        if ((unsigned int)instLine->srcAdd == 2 || (unsigned int)instLine->dstAdd == 2)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        break;
    case 2:
        if ((unsigned int)instLine->srcAdd == 2)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if (((unsigned int)instLine->dstAdd) == 2 || ((unsigned int)instLine->dstAdd) == 0)
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 4:
        if ((unsigned int)instLine->srcAdd != 1)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if (((unsigned int)instLine->dstAdd == 0) || ((unsigned int)instLine->dstAdd == 2))
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 5:
        if ((unsigned int)instLine->srcAdd != 0)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if (((unsigned int)instLine->dstAdd == 0) || ((unsigned int)instLine->dstAdd == 2))
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 9:
        if ((unsigned int)instLine->srcAdd != 0)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if ((unsigned int)instLine->dstAdd == 0 || (unsigned int)instLine->dstAdd == 3)
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 12:
        if ((unsigned int)instLine->srcAdd != 0)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if ((unsigned int)instLine->dstAdd == 2 || (unsigned int)instLine->dstAdd == 0)
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 13:
        if ((unsigned int)instLine->srcAdd != 0)
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        else
        {
            if ((unsigned int)instLine->dstAdd == 2)
                error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        }
        break;
    case 14:
        if (((unsigned int)instLine->srcAdd != 0) || ((unsigned int)instLine->dstAdd != 0))
            error("Illegal addressing mode.\t\t\t|in line %d", lineNum);
        break;
    }
}
