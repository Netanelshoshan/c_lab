/*
 * =====================================================================================================
 * name        : mmn_12/common.h
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : Header file for adjacency.c
 * =====================================================================================================
 */

#include <stdio.h>

#define N 11 /*size of matrix*/

/*prototype declarations*/
enum boolean {
    FALSE = 0, TRUE = 1
};
typedef int adjmat[N][N];

enum boolean path(adjmat AdjacencyMatrix, int u, int v);

int rootFinder(adjmat AdjacencyMatrix, int v);

void buildM(adjmat AdjacencyMatrix);

void printM(adjmat AdjacencyMatrix);

void yellow();

void grey();

void red();

void green();

void reset();

