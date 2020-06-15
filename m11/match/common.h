/*
 * =====================================================================================================
 * name        : mmn_11/match/common.h
 * author      : Netanel Shoshan ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : Header file for match.c.
 * =====================================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*constants & prototype decleration*/

#define MAXCHAR 100
#define MAXFILE 1000

int charFinder(char *s, char c, int k);

void cp_arr(char src[], char dst[]);

int my_strcmp(char str1[], char str2[]);
