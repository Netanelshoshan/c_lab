/*
  * @ Netanel Shoshan @
  *
  * first stage parsing functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "system.h"
#include "stages.h"
#include "utils.h"

/* will call other parsing function in order to parse the current line. */
void firstStage(File_input *file, int numOfLines) {
    /*vars decelerations*/
    int i;
    char *label;
    errorFlag = 0, ic = 0, dc = 0, externCnt = 0, entryCnt = 0; /* init the counters with 0 values*/

    for (i = 0; i < numOfLines; i++) {
        label = isLabel(file + i);              /* label deceleration validation (LOOP,MAIN, etc..)*/
        spaceTrimmer(&(file[i].line));                  /* space trimmer */
        if (file[i].line[0] == '.')                     /* handling command deceleration */
            commandHandler(&file[i], label);
        else {
            if (label)                                  /* if it's a label */
                tmpNode = install(label, ic, instTable);
            instructionHandler(&file[i]);               /* parse everything after it. */
        }
    }
}

/* Label parsing function.
 * will install the label based on kind. */
void commandHandler(File_input *codeLine, char *symbol) {
    if (strncmp(codeLine->line, ".string", sizeof(".string") - 1) == 0) { /* .string handling */
        /* Will occur only if the line starts with "label:" */
        if (symbol)
            tmpNode = install(symbol, dc, dataSymTable);

        /*advance the pointer beyond deceleration and parse the string */
        codeLine->line += (sizeof(".string") - 1);
        stringHandler(codeLine);
    }
    else if (strncmp((*codeLine).line, ".data", sizeof(".data") - 1) == 0) { /*.data handling */
        /* Will occur only if the line starts with "label:" */
        if (symbol)
            tmpNode = install(symbol, dc, dataSymTable);

        /*advance the pointer beyond deceleration and parse the data */
        codeLine->line += (sizeof(".data") - 1);
        dataHandler(codeLine);
    }
    else if (strncmp(codeLine->line, ".entry", sizeof(".entry") - 1) == 0) { /*.entry handling*/
        /*advance the pointer beyond deceleration and parse the entry */
        codeLine->line += (sizeof(".entry") - 1);
        entryHandler(codeLine);
    }
    else if (strncmp(codeLine->line, ".extern", sizeof(".extern") - 1) == 0) { /*.extern handling */
        /*advance the pointer beyond deceleration and parse the extern label */
        codeLine->line += (sizeof(".extern") - 1);
        externHandler(codeLine);
    }
    else {
        error(RED"*commandHandler:"RST" Unrecognized command %s .\t|in line %d.", symbol,
              (*codeLine).lineNum); /* error handling */
    }
    /* mark the line as done. no need to parse it again */
    codeLine->isDone = 1;
}

/* Data parsing function that will parse data values. */
void dataHandler(File_input *codeLine) {
    /*var declerations */
    int sign = 1, num = 0;
    char moreFlag = 0;

    /* run through the line */
    while ((*codeLine).line[0] != '\n') {
        num = 0;
        spaceTrimmer(&((*codeLine).line));      /* trim spaces */
        if (codeLine->line[0] == '-') {         /* negative value validation, flag will raised if needed */
            sign = -1;
            (codeLine->line)++;
        }
        /* digit validation */
        if (!isdigit((*codeLine).line[0]))
            error(RED"*dataHandler: "RST" Wrong .data format - %s. Numbers expected.\t| in line %d.", (*codeLine).line,
                  (*codeLine).lineNum);
        /*will run until ',' reached and multiply the number by 10 so we could handle it */
        else {
            while (isdigit(codeLine->line[0])) {
                num *= 10;
                num += ((*codeLine).line[0] - '0');
                (codeLine->line)++;
            }
            dataArray[dc++] = numberToData(num * sign, X);      /* Add the number to the data array */
        }
        spaceTrimmer(&((*codeLine).line)); /* trim spaces */
        if (codeLine->line[0] == ',') { /* validate if we excpecting more */
            moreFlag = 1;
            (codeLine->line)++;
        }
        else /* if not */
            moreFlag = 0;
    }
    /* another number is expected but there just a ','*/
    if (moreFlag) {
        error(RED"*dataHandler:"RST" number is expected after \',\'\t|in line %d.", (*codeLine).lineNum);
    }
}

/* string parsing function that will parse the string according to the
 * rules and add it to the dataArray.*/
void stringHandler(File_input *codeLine) {
    spaceTrimmer(&((*codeLine).line)); /*trim junk spaces*/

    if (codeLine->line[0] != '\"') { /* comma validation*/
        error(RED"*stringHandler: "RST".string deceleration must start with '\"'\t|in line %d.",
              (*codeLine).lineNum);
        return;
    }

    (codeLine->line)++; /*advancing the ptr after the first \" and parsing */

    while (codeLine->line[0] != '\"' && codeLine->line[0] != '\0') {
        if (codeLine->line[0] == '#')
            (codeLine->line)++;
        dataArray[dc++] = charToData(codeLine->line[0]); /* add the char to the data array */
        (codeLine->line)++;
    }
    /*end of string has been reached, register '\0' to the dataArray by the
     * end of the string */
    if (codeLine->line[0] == '\"') {
        dataArray[dc++] = charToData('\0');
    }
    else { /* string does not end with \" */
        error(RED"*stringHandler: "RST".string deceleration must end with \"\t|in line %d.", (*codeLine).lineNum);
        return;
    }
}

/* entry parsing function that will register the entry label into the entryLabelArray.*/
void entryHandler(File_input *codeLine) {

    codeLine->line = strtok(codeLine->line, " \t\n"); /* isolate the entry label */

    if (!codeLine->line) { /* empty declaration */
        error(RED"*entryHandler: "RST"No label provided after .entry command.\t|in line %d", (*codeLine).lineNum);
        return;
    }
    /* put out a message for the new entry*/
    printf("*INFORM: " GRN ".entry" RST " decleration" GRN " \"%s\" " RST "has been made in line %d.\n",
           codeLine->line,
           (*codeLine).lineNum);

    entryArray[entryCnt++] = makeDup(codeLine->line);             /* copy the label to entryLabelArray */
    codeLine->line = strtok(NULL, " \t\n");              /* trim the rest*/

    if (codeLine->line) {                                       /*if there's more after the command */
        error(RED"*entryHandler: "RST"Multiple labels provided after .entry command.\t| in line %d.",
              (*codeLine).lineNum);
    }
}

/* extern parsing function that will parse extern label*/
void externHandler(File_input *codeLine) {

    codeLine->line = strtok(codeLine->line, " \t\n"); /* isolate extern label */

    if (!codeLine->line) {/* empty declaration */
        error(RED"*externHandler:"RST" No external label provided after .extern command.\t| in line %d.",
              (*codeLine).lineNum);
        return;
    }
    /* put out a message for the new external*/
    printf("*INFORM: " YEL ".extern" RST " decleration" YEL " \"%s\" "RST"has been made in line %d.\n",
           codeLine->line,
           (*codeLine).lineNum);

    install(codeLine->line, '0', externalTable);    /*install label into externalTable with 0 value */
    externCnt++;                                        /* advance the external counter by 1*/
    codeLine->line = strtok(NULL, " \t\n");     /*trim the rest*/

    if (codeLine->line) {                               /*if there's more after the command */
        error(RED"*externHandler: "RST"Multiple labels provided after .extern command.\t| in line %d.",
              (*codeLine).lineNum);
    }
}

/* function that return the number of expected operand based on the name given */
char expectedOperands(char *name) {
    if (strcmp(name, "rts") == 0 || strcmp(name, "stop") == 0) {
        return 0;
    }
    else if (strcmp(name, "mov") == 0 || strcmp(name, "cmp") == 0 || strcmp(name, "add") == 0 ||
             strcmp(name, "sub") == 0 || strcmp(name, "lea") == 0) {
        return 2;
    }
    else
        return 1;
}

/**
 * Instruction line parser.
 * the fuction will handle:
 * - opcode validation
 * - funcT
 * - initialize A bit with 1.
 * - src/dst addr methods.
 * - finally will increment ic based on flags that were puted in some places in the function.
 */
void instructionHandler(File_input *codeLine) {
    int localCnt = 0, numOfExpOperands, functValue; /*vars declarations*/

    /*----------OPCODE VALIDATION----------*/
    codeLine->line = strtok(codeLine->line, "\t\n ");   /* isolating opcode */
    tmpNode = lookup(codeLine->line, opcodeTable);          /* opcode validation */
    if (!tmpNode) {                                        /* opcode , comment & new line validation */
        if ((codeLine->line[0] == ';') || (codeLine->line[0] == '\n')) {
            codeLine->isDone = 1;
            return;
        }
        error(RED"*InstructionParsing: "RST"Invalid instruction "YEL"\"%s\""RST".\t| in line %d", codeLine->line,
              (*codeLine).lineNum);
        return;
    }

    numOfExpOperands = expectedOperands(codeLine->line);                            /*grab the number of exp operands*/
    codeLine->moreParsing = numOfExpOperands;                                       /* Number of expected operands */
    functValue = functFetcher(codeLine->line, opcodeTable);                         /* grab thg funct value */
    codeLine->instruction = (Instruction *) malloc(sizeof(Instruction) * 9); /* allocate enough space*/
    initInstructionLine(codeLine->instruction);      /* Initialize instruction line with 0 values */
    if (codeLine->instruction == NULL)                                              /* validating for mem allocation.*/
    {
        error(RED"*SYSTEM: "RST"Memory allocation Failed.\t| in line %d", (*codeLine).lineNum);
        exit(1);
    }
    localCnt++;                                   /* advance the local counter for ic */
    codeLine->instruction->opCode = tmpNode->val; /* assign the opcode val to Instruction*/
    codeLine->instruction->funct = functValue;    /* same with funct */
    codeLine->instruction->A = 1;                 /*initialize A bit with 1.*/

    /* determine if a line needs more than one parsing*/
    if (numOfExpOperands == 0)
        codeLine->moreParsing = 1;
    if (numOfExpOperands > 1)
        codeLine->moreParsing = numOfExpOperands - 1;


    /* advance the pointer pass the command and trim space junk in order to get to the values*/
    while (codeLine->line[0] != '\0')
        (codeLine->line)++;
    (codeLine->line)++;
    spaceTrimmer(&(codeLine->line));
    codeLine->line = strtok(codeLine->line, " \t,\n");

    /*Addressing method parsing */
    while (numOfExpOperands-- > 0) {
        /* in each iteration decide which is the src/dst and copy it to the src/dstOpr variable */
        if (numOfExpOperands == 1)
            codeLine->srcOpr = makeDup(codeLine->line);
        else
            codeLine->dstOpr = makeDup(codeLine->line);

        switch (codeLine->line[0]) {
            case '#': /*Immediate addressing */
                localCnt++;
                if (numOfExpOperands == 1)
                    codeLine->instruction->srcAdd = 0;
                else
                    codeLine->instruction->dstAdd = 0;
                break;
            case 'r': /* Direct reg addressing  */
                (codeLine->line)++;
                /*src/dstReg value will be determined based on the register values without the 'r' char
                     * in that case, srcOpr will be freed because there is no need to parse it again.
                     * SAME GOES FOR DESTINATION HANDLING */
                if ((codeLine->line[0] >= '0') && ((codeLine->line[0] <= '7'))) {
                    if (numOfExpOperands == 1) {
                        codeLine->instruction->srcReg = codeLine->line[0] - '0';
                        (codeLine->srcOpr) = NULL;
                        codeLine->instruction->srcAdd = 3;
                    }
                    else { /*HERE*/
                        codeLine->instruction->dstReg = codeLine->line[0] - '0';
                        (codeLine->dstOpr) = NULL;
                        codeLine->instruction->dstAdd = 3;
                    }
                    break;
                }
            case '&': /* Relative addressing */
                localCnt++;
                codeLine->instruction->dstAdd = 2;
                break;
            default: /* Direct addressing */
                localCnt++;
                if (numOfExpOperands == 1)
                    codeLine->instruction->srcAdd = 1;
                else
                    codeLine->instruction->dstAdd = 1;
        }
        codeLine->line = strtok(NULL, " \t\n"); /*clean the rest*/
    }
    /* increment the ic based on localCnt * moreParsing value */
    ic += (localCnt * codeLine->moreParsing);
}

/* isLabel will check if the line starts with a label, like "FSD:"*/
char *isLabel(File_input *codeLine) {
    char *tmpStr = (codeLine->line); /* grab a copy of it*/
    /* run through the line */
    while (*(codeLine->line) != '\0') {
        if (*(codeLine->line) == ':') { /* if it a label decleration */
            *((codeLine->line)++) = '\0'; /*put a null char in front of it and send the TMPSTR */
            if (!isalpha(tmpStr[0])) { /* label deceleration validation*/
                error(RED"*isLabel: "RST"label must start with character.\t| in line %d.", (*codeLine).lineNum);
            }
            return tmpStr;
        }
        (codeLine->line)++;
    }
    codeLine->line = tmpStr; /*if none was found , assign back the copy that was created */
    return NULL;
}
