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
        spaceTrimmer(&(file[i].line.content));                  /* space trimmer */
        if (file[i].line.content[0] == '.')                     /* handling command deceleration */
            commandHandler(&file[i], label);
        else {
            if (label)                                  /* if it's a label */
                tmpNode = install(label, ic, instTable);
            instructionHandler(&file[i]);               /* parse everything after it. */
        }
    }
}

/* Label parsing function.
 * will install the label based on the kind of the label. */
void commandHandler(File_input *input, char *symbol) {
    if (strncmp(input->line.content, ".string", sizeof(".string") - 1) == 0) { /* .string handling */
        /* Will occur only if the line starts with "label:" */
        if (symbol)
            tmpNode = install(symbol, dc, dataSymbolsTable);

        /*advance the pointer beyond deceleration and parse the string */
        input->line.content += (sizeof(".string") - 1);
        stringHandler(input);
    }
    else if (strncmp(input->line.content, ".data", sizeof(".data") - 1) == 0) { /*.data handling */
        /* Will occur only if the line starts with "label:" */
        if (symbol)
            tmpNode = install(symbol, dc, dataSymbolsTable);

        /*advance the pointer beyond deceleration and parse the data */
        input->line.content += (sizeof(".data") - 1);
        dataHandler(input);
    }
    else if (strncmp(input->line.content, ".entry", sizeof(".entry") - 1) == 0) { /*.entry handling*/
        /*advance the pointer beyond deceleration and parse the entry */
        input->line.content += (sizeof(".entry") - 1);
        entryHandler(input);
    }
    else if (strncmp(input->line.content, ".extern", sizeof(".extern") - 1) == 0) { /*.extern handling */
        /*advance the pointer beyond deceleration and parse the extern label */
        input->line.content += (sizeof(".extern") - 1);
        externHandler(input);
    }
    else {
        error(RED"*commandHandler:"RST" Unrecognized command %s .\t|in line %d.", symbol,
              (*input).lineNum); /* error handling */
    }
    /* mark the line as done. no need to parse it again */
    input->line.isDone = 1;
}

/* Data parsing function that will parse data values. */
void dataHandler(File_input *input) {
    /*var declerations */
    int sign = 1, num = 0;
    char moreFlag = 0;

    /* run through the line */
    while ((*input).line.content[0] != '\n') {
        num = 0;
        spaceTrimmer(&((*input).line.content));      /* trim spaces */
        if (input->line.content[0] == '-') {         /* negative value validation, flag will raised if needed */
            sign = -1;
            (input->line.content)++;
        }
        /* digit validation */
        if (!isdigit((*input).line.content[0]))
            error(RED"*dataHandler: "RST" Wrong .data format - %s. Numbers expected.\t| in line %d.", (*input).line,
                  (*input).lineNum);
        /*will run until ',' reached and multiply the number by 10 so we could handle it */
        else {
            while (isdigit(input->line.content[0])) {
                num *= 10;
                num += ((*input).line.content[0] - '0');
                (input->line.content)++;
            }
            dataArray[dc++] = numberToData(num * sign, X);      /* Add the number to the data array */
        }
        spaceTrimmer(&((*input).line.content)); /* trim spaces */
        if (input->line.content[0] == ',') { /* validate if we excpecting more */
            moreFlag = 1;
            (input->line.content)++;
        }
        else /* if not */
            moreFlag = 0;
    }
    /* another number is expected but there just a ','*/
    if (moreFlag) {
        error(RED"*dataHandler:"RST" number is expected after \',\'\t|in line %d.", (*input).lineNum);
    }
}

/* string parsing function that will parse the string according to the
 * rules and add it to the dataArray.*/
void stringHandler(File_input *input) {
    spaceTrimmer(&((*input).line.content)); /*trim junk spaces*/

    if (input->line.content[0] != '\"') { /* comma validation*/
        error(RED"*stringHandler: "RST".string deceleration must start with '\"'\t|in line %d.",
              (*input).lineNum);
        return;
    }

    (input->line.content)++; /*advancing the ptr after the first \" and parsing */

    while (input->line.content[0] != '\"' && input->line.content[0] != '\0') {
        if (input->line.content[0] == '#')
            (input->line.content)++;
        dataArray[dc++] = charToData(input->line.content[0]); /* add the char to the data array */
        (input->line.content)++;
    }
    /*end of string has been reached, register '\0' to the dataArray by the
     * end of the string */
    if (input->line.content[0] == '\"') {
        dataArray[dc++] = charToData('\0');
    }
    else { /* string does not end with \" */
        error(RED"*stringHandler: "RST".string deceleration must end with \"\t|in line %d.", (*input).lineNum);
        return;
    }
}

/* entry parsing function that will register the entry label into the entryLabelArray.*/
void entryHandler(File_input *pInput) {

    pInput->line.content = strtok(pInput->line.content, " \t\n"); /* isolate the entry label */

    if (!pInput->line.content) { /* empty declaration */
        error(RED"*entryHandler: "RST"No label provided after .entry command.\t|in line %d", (*pInput).lineNum);
        return;
    }
    /* put out a message for the new entry*/
    printf("*INFORM: " GRN ".entry" RST " decleration" GRN " \"%s\" " RST "has been made in line %d.\n",
           pInput->line.content,
           (*pInput).lineNum);

    entryTable[entryCnt++] = makeDup(pInput->line.content);             /* copy the label to entryLabelArray */
    pInput->line.content = strtok(NULL, " \t\n");              /* trim the rest*/

    if (pInput->line.content) {                                       /*if there's more after the command */
        error(RED"*entryHandler: "RST"Multiple labels provided after .entry command.\t| in line %d.",
              (*pInput).lineNum);
    }
}

/* extern parsing function that will parse extern label*/
void externHandler(File_input *pInput) {

    pInput->line.content = strtok(pInput->line.content, " \t\n"); /* isolate extern label */

    if (!pInput->line.content) {/* empty declaration */
        error(RED"*externHandler:"RST" No external label provided after .extern command.\t| in line %d.",
              (*pInput).lineNum);
        return;
    }
    /* put out a message for the new external*/
    printf("*INFORM: " YEL ".extern" RST " decleration" YEL " \"%s\" "RST"has been made in line %d.\n",
           pInput->line.content,
           (*pInput).lineNum);

    install(pInput->line.content, '0', extTable);    /*install label into extTable with 0 value */
    externCnt++;                                        /* advance the external counter by 1*/
    pInput->line.content = strtok(NULL, " \t\n");     /*trim the rest*/

    if (pInput->line.content) {                               /*if there's more after the command */
        error(RED"*externHandler: "RST"Multiple labels provided after .extern command.\t| in line %d.",
              (*pInput).lineNum);
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
void instructionHandler(File_input *input) {
    int localCnt = 0, numOfExpOperands, functValue; /*vars declarations*/

    /*----------OPCODE VALIDATION----------*/
    input->line.content = strtok(input->line.content, "\t\n ");   /* isolating opcode */
    tmpNode = lookup(input->line.content, opcodeTable);          /* opcode validation */
    if (!tmpNode) {                                        /* opcode , comment & new line validation */
        if ((input->line.content[0] == ';') || (input->line.content[0] == '\n')) {
            input->line.isDone = 1;
            return;
        }
        error(RED"*InstructionParsing: "RST"Invalid instruction "YEL"\"%s\""RST".\t| in line %d", input->line.content,
              (*input).lineNum);
        return;
    }

    numOfExpOperands = expectedOperands(input->line.content);                            /*grab the number of exp operands*/
    input->line.moreParsing = numOfExpOperands;                                       /* Number of expected operands */
    functValue = functFetcher(input->line.content, opcodeTable);                         /* grab thg funct value */
    input->instruction = (Instruction *) malloc(sizeof(Instruction) * 9); /* allocate enough space*/
    initInstructionLine(input->instruction);      /* Initialize instruction line with 0 values */
    if (input->instruction == NULL)                                              /* validating for mem allocation.*/
    {
        error(RED"*SYSTEM: "RST"Memory allocation Failed.\t| in line %d", (*input).lineNum);
        exit(1);
    }
    localCnt++;                                   /* advance the local counter for ic */
    input->instruction->opCode = tmpNode->val; /* assign the opcode val to Instruction*/
    input->instruction->funct = functValue;    /* same with funct */
    input->instruction->A = 1;                 /*initialize A bit with 1.*/

    /* determine if a line needs more than one parsing*/
    if (numOfExpOperands == 0)
        input->line.moreParsing = 1;
    if (numOfExpOperands > 1)
        input->line.moreParsing = numOfExpOperands - 1;


    /* advance the pointer pass the command and trim space junk in order to get to the values*/
    while (input->line.content[0] != '\0')
        (input->line.content)++;
    (input->line.content)++;
    spaceTrimmer(&(input->line.content));
    input->line.content = strtok(input->line.content, " \t,\n");

    /*Addressing method parsing */
    while (numOfExpOperands-- > 0) {
        /* in each iteration decide which is the src/dst and copy it to the src/dstOpr variable */
        if (numOfExpOperands == 1)
            input->srcOpr = makeDup(input->line.content);
        else
            input->dstOpr = makeDup(input->line.content);

        switch (input->line.content[0]) {
            case '#': /*Immediate addressing */
                localCnt++;
                if (numOfExpOperands == 1)
                    input->instruction->srcAdd = 0;
                else
                    input->instruction->dstAdd = 0;
                break;
            case 'r': /* Direct reg addressing  */
                (input->line.content)++;
                /*src/dstReg value will be determined based on the register values without the 'r' char
                     * in that case, srcOpr will be freed because there is no need to parse it again.
                     * SAME GOES FOR DESTINATION HANDLING */
                if ((input->line.content[0] >= '0') && ((input->line.content[0] <= '7'))) {
                    if (numOfExpOperands == 1) {
                        input->instruction->srcReg = input->line.content[0] - '0';
                        (input->srcOpr) = NULL;
                        input->instruction->srcAdd = 3;
                    }
                    else { /*HERE*/
                        input->instruction->dstReg = input->line.content[0] - '0';
                        (input->dstOpr) = NULL;
                        input->instruction->dstAdd = 3;
                    }
                    break;
                }
            case '&': /* Relative addressing */
                localCnt++;
                input->instruction->dstAdd = 2;
                break;
            default: /* Direct addressing */
                localCnt++;
                if (numOfExpOperands == 1)
                    input->instruction->srcAdd = 1;
                else
                    input->instruction->dstAdd = 1;
        }
        input->line.content = strtok(NULL, " \t\n"); /*clean the rest*/
    }
    /* increment the ic based on localCnt * moreParsing value */
    ic += (localCnt * input->line.moreParsing);
}

/* isLabel will check if the line starts with a label, like "FSD:"*/
char *isLabel(File_input *input) {
    char *tmpStr = (input->line.content); /* grab a copy of it*/
    /* run through the line */
    while (*(input->line.content) != '\0') {
        if (*(input->line.content) == ':') { /* if it a label decleration */
            *((input->line.content)++) = '\0'; /*put a null char in front of it and send the TMPSTR */
            if (!isalpha(tmpStr[0])) { /* label deceleration validation*/
                error(RED"*isLabel: "RST"label must start with character.\t| in line %d.", (*input).lineNum);
            }
            return tmpStr;
        }
        (input->line.content)++;
    }
    input->line.content = tmpStr; /*if none was found , assign back the copy that was created */
    return NULL;
}
