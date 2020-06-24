/*
 * @ netanel Shoshan @
 *
 * prototypes for assisting functions.
 */

#ifndef UTILS_H
#define UTILS_H

/*------- base conversion -------*/
void reverse(char *str, int length);                            /* to reverse a string */
char *baseConvertor(int num, int base, char *result, int pad);  /* base converter with padding option*/
char *padWithZees(char *s, int padLen);                         /* zero's paddler by request */

/*-------------- for fi and second stage --------- */
void spaceTrimmer(char **str);                          /* space trimmer */
char *makeDup(const char *s);                           /* making dup of a string */

/*----- used in initAsm in order to skip new lines before 1st&2nd stage -------*/
int isNewLine(char **str);                              /* new line indicator */
int isComment(char **str);                              /* comment indicator */

/*--------errors and status indicators ----------*/
void error(char *msg, ...);                          /* error handler */
void status(char *fileName, int eflag, int isStart);    /* status for interpretation indicator*/

#endif
