/*
 * =========================================================================================
 * name        : mmn_22/commmon.h
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : header file containing prototype declerations, states enumrations , constant
 * declerations and the import of all libraries necessary for the mycomp.c and complex.c.
 * =========================================================================================
 */
#ifndef MMN_22V1_1_COMMON_H
#define MMN_22V1_1_COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


/*===============================GLOBAL DEFINITIONS==================================*/
#define PROG_NAME      "COMPLEX CALCULATOR"
#define VERSION        "MMN_22 v1.2, 316393321"
#define LINE           "============================================================="

/*========================INPUT-STATES,ERRORS,SYSTEM-MSGS=============================== */

enum inputState {
    WAIT_FOR_CMD_NAME, WAIT_FOR_VAR, WAIT_FOR_PARAMS, ERROR
};
enum inputParams {
    VAR, ARGS, SCALAR, NONE
};
enum errorMsgs {
    CMD_NOT_EXIST, INVALID_ARGS, CMD, VAR_NOT_EXIST, LOW_ARGS, EXTRA_AFTER, MISSING_COMM, ERROR_HELP,EOF_
};
enum systemMsgs {
    START, HELP, HALT
};

/*============================mycomp.c PROTOTYPE-DECELERATIONS=========================*/
void startUserInterface();

void errorHandler(char *command, int msgIndex);

void clearBuffer();

int stateAsCmd(int indexOfCmd);

void clearCommandString(char *command);

void remove_spaces(char *s);

/*=====================STUFF FOR THE UI============================*/
#define FOREVER         for(;;)
#define MAX_BUFFER     30
#define MIN_COMMAND     4
#define MAX_SCALAR      30
#define ENTER_KEY       10
#define MIN_ARGS        3

#endif /*MMN_22V1_1_COMMON_H*/
