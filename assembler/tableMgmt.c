/**
 * @ Netanel Shoshan @
 * 
 * contains functions for tables management.
 * more info about the functions can be found in system.h
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "utils.h"
#include "structs.h"

/* Hashing function.*/
unsigned int hash(char *s)
{
    unsigned int salt;
    for (salt = 0; *s != '\0'; s++)
        salt = *s + 31 * salt;
    return salt % HASHSIZE;
}

/* Function that search for string in array of sysNode ptr's.*/
sysNode *lookup(char *string, sysNode **table)
{
    sysNode *ptr;
    for (ptr = table[hash(string)]; ptr != NULL; ptr = ptr->next)
        if (strcmp(string, ptr->name) == 0)
            return ptr; /* string found */

    return NULL; /* string not found */
}

/* Function that install given name and value in desired table.*/
sysNode *install(char *name, int val, sysNode **table)
{

    sysNode *ptr;
    unsigned int tmpHash;
    if ((ptr = lookup(name, table)) == NULL)
    { /* If the name wasn't found */

        /* allocate space for the new entry */
        ptr = (sysNode *)malloc(sizeof(*ptr));

        /* empty string or malloc cant allocate requested memory */
        if (ptr == NULL)
        {
            fprintf(stderr, "*install: Memory allocation failed for %s\n", name);
            exit(1);
        }
        if ((ptr->name = makeDup(name)) == NULL)
            return NULL;

        /* hash the current name and assign it in the tables */
        tmpHash = hash(name);
        ptr->next = table[tmpHash];
        table[tmpHash] = ptr;
    }
    else
    {
        free((void *)ptr->name); /* free previous name */
    }
    ptr->val = val;
    return ptr;
}

/* Function that install all opcodes when initOpCodes() is invoked. */
sysNode *installOpcode(char *name, int funct, int opcVal, sysNode **table)
{
    sysNode *ptr;
    unsigned int tmpHash;
    if ((ptr = lookup(name, table)) == NULL)
    { /* If the name wasn't found */

        /* allocate space for the new entry */
        ptr = (sysNode *)malloc(sizeof(*ptr));

        /* empty string or malloc cant allocate requested memory */
        if (ptr == NULL)
        {
            fprintf(stderr, "*installOpcode: Memory allocation failed for opc:%d,funct:%d\n", opcVal, funct);
            exit(1);
        }
        if ((ptr->name = makeDup(name)) == NULL)
            return NULL;

        /* hash the current name and assign it in the tables */
        tmpHash = hash(name);
        ptr->next = table[tmpHash];
        table[tmpHash] = ptr;
    }
    else
    {
        free((void *)ptr->name);
    }
    ptr->val = opcVal;
    ptr->funct = funct;
    return ptr;
}

/* Function that retrieve the funct by given command's name. */
int functFetcher(char *name, sysNode **table)
{
    sysNode *ptr;
    for (ptr = table[hash(name)]; ptr != NULL; ptr = ptr->next)
    {
        if (strcmp(name, ptr->name) == 0)
            return ptr->funct;
    }
    return 0;
}

/**
 * opcode initializer.
 * used when initAsm is called.
 */
void initOpCodes()
{
    tmpNode = installOpcode("mov", 0, 0, opcodeTable);
    tmpNode = installOpcode("cmp", 0, 1, opcodeTable);
    tmpNode = installOpcode("add", 1, 2, opcodeTable);
    tmpNode = installOpcode("sub", 2, 2, opcodeTable);
    tmpNode = installOpcode("lea", 0, 4, opcodeTable);
    tmpNode = installOpcode("clr", 1, 5, opcodeTable);
    tmpNode = installOpcode("not", 2, 5, opcodeTable);
    tmpNode = installOpcode("inc", 3, 5, opcodeTable);
    tmpNode = installOpcode("dec", 4, 5, opcodeTable);
    tmpNode = installOpcode("jmp", 1, 9, opcodeTable);
    tmpNode = installOpcode("bne", 2, 9, opcodeTable);
    tmpNode = installOpcode("jsr", 3, 9, opcodeTable);
    tmpNode = installOpcode("red", 0, 12, opcodeTable);
    tmpNode = installOpcode("prn", 0, 13, opcodeTable);
    tmpNode = installOpcode("rts", 0, 14, opcodeTable);
    tmpNode = installOpcode("stop", 0, 15, opcodeTable);
}

/**
 * Converts given number into 24 bit data.
 * The function will also get required A,R,E and set the the
 * A,R,E field of the data as needed.
 */
data numberToData(int num, int are)
{

    /* mask for 2^21-1.Filling all bits with 1111...11 */
    unsigned int mask = 0x1FFFFF;
    data d;
    initDataLine(&d); /* init d obj with 0 value. */

    if (num >= 0)
    { /*pos val handling */
        d.line = num & mask;
    }
    else
    { /*negative vals handling */
        d.line = (num * -1) & mask;
        d.line = ~(d.line);
        (d.line)++;
    }
    /*A,R,E handling*/
    switch (are)
    {
    case X: /* if it's a number */
        return d;
    case ABSOLUTE:
        d.line = d.line << 3;
        d.line |= 1 << 2;
        break;
    case RELOCATABLE:
        d.line = d.line << 3;
        d.line |= 1 << 1;
        break;
    case EXTERNAL:
        d.line = d.line << 3;
        d.line |= 1;
        break;
    }

    return d;
}

/* data initializer */
void initDataLine(data *dataPtr)
{
    dataPtr->line = 0;
}

/* converts char to 24 bit data line */
data charToData(char ch)
{
    data ptr;

    initDataLine(&ptr); /* init ptr */
    ptr.line = ch;

    return ptr; /* return the pointer */
}

/* Instruction line initializer */
void initInstructionLine(Instruction *inst)
{
    inst->E = 0;
    inst->R = 0;
    inst->A = 0;
    inst->funct = 0;
    inst->dstReg = 0;
    inst->dstAdd = 0;
    inst->srcReg = 0;
    inst->srcAdd = 0;
    inst->opCode = 0;
}

/* mask for convert_IL_to_data function. */
unsigned char mask[] = {0x0, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF};

/* Converts Instruction object to 24 data inst.
 * It creates 24 bit data inst and
 * insert the Instruction obj into it using a special mask. */
data convert_IL_to_data(Instruction inst)
{
    data d;
    initDataLine(&d); /*initialize the 24 bit data line. */

    d.line |= mask[TYPE_WIDTH] << EXTERNAL_OFFSET & inst.E << EXTERNAL_OFFSET;
    d.line |= mask[TYPE_WIDTH] << RELOCATABLE_OFFSET & inst.R << RELOCATABLE_OFFSET;
    d.line |= mask[TYPE_WIDTH] << ABSOLUTE_OFFSET & inst.A << ABSOLUTE_OFFSET;
    d.line |= mask[FUNCT_WIDTH] << FUNCT_OFFSET & inst.funct << FUNCT_OFFSET;
    d.line |= mask[DREG_WIDTH] << DREG_OFFSET & inst.dstReg << DREG_OFFSET;
    d.line |= mask[DADDR_WIDTH] << DADDR_OFFSET & inst.dstAdd << DADDR_OFFSET;
    d.line |= mask[SREG_WIDTH] << SREG_OFFSET & inst.srcReg << SREG_OFFSET;
    d.line |= mask[SADDR_WIDTH] << SADDR_OFFSET & inst.srcAdd << SADDR_OFFSET;
    d.line |= mask[OPCODE_WIDTH] << OPCODE_OFFSET & inst.opCode << OPCODE_OFFSET;

    return d; /* return the converted instruction line */
}
