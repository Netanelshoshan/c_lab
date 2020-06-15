/*
 * =====================================================================================================
 * name        : mmn_12/adjancency.c
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : path function gets root(u) and node(v) and determine if there's a path from u to v.
 * =====================================================================================================
 */

#include "common.h"
/*
 * recursive function to check if there's a path from u to v
 * will use the rootFinder as a helper func.
 */
enum boolean path(adjmat AdjacencyMatrix, int u, int v) {
    int root;
    /*base conditions*/
    if (u >= N || v >= N || u < 0 || v < 0)
        return FALSE;

    if ((AdjacencyMatrix[u][v] == '1') || (u == v))
        return TRUE;

    /*calling the assisting func*/
    root = rootFinder(AdjacencyMatrix, v);

    return path(AdjacencyMatrix, u, root);
}

/*
 * This is an assisting function for the path function.
 * will check if 'u' that was provided by the user have a father.
 * In case we found a '1' value, it means that there's a path from u to v,
 * will return i, otherwise will return -1.
*/
int rootFinder(adjmat AdjacencyMatrix, int v) {
    int i;
    for (i = 0; i < N; i++) {
        if (AdjacencyMatrix[i][v] == '1') {
            return i;
        }
    }
    return -1;
}

/* Function that initialize the matrix with the values from stdin */
void buildM(adjmat AdjacencyMatrix) {
    int i, j;
    char tempc;/*save the input temporarily*/

    printf("\nInitializing Matrix:\n");
    green();
    printf("The current Matrix size is %d X %d\n", N, N);
    reset();
    printf("Please provide the values as 0/1 %d times:\n", N * N);

    /**/
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            while ((tempc = getchar()) == ' ' || (tempc == '\t') || (tempc == '\n'));
            AdjacencyMatrix[i][j] = tempc;
        }
    }
}

/* helper Printing function */
void printM(adjmat AdjacencyMatrix) {
    int u, v, col;

    printf("\n\nThe Matrix provided:\n\n");

    /* columns number printing */
    for (col = 0; col < N; col++) {
        if (col == 0)
            printf("       %d", col);
        else if (col > 0 && col < 10)
            printf("   %d", col);
        else
            printf("  %d", col);
    }

    printf("\n");

    /* rows numbers including values and seperators */
    for (u = 0; u < N; u++) {
        if (u < 10)
            printf("\n %d   | ", u);
        else
            printf("\n %d  | ", u);
        for (v = 0; v < N; v++) {
            if (AdjacencyMatrix[u][v] == '1') {
                red();
                printf("%c", AdjacencyMatrix[u][v]);
                reset();
                printf(" | ");
            } else {
                printf("%c", AdjacencyMatrix[u][v]);
                printf(" | ");
            }
        }
    }
    printf("\n");
}

/* color indicators for the results.*/
void red() {
    printf("\033[1;31m");
}

void grey() {
    printf("\033[1;37m");
}

void green() {
    printf("\033[1;32m");
}

void yellow() {
    printf("\033[1;33m");
}

void reset() {
    printf("\033[0m");
}

/* main */
int main() {

    /* Variables declarations*/
    int u, v, s;
    enum boolean res;
    adjmat AdjacencyMatrix;
    buildM(AdjacencyMatrix); /* Initializing the matrix */
    printM(AdjacencyMatrix); /* printing function*/

    /*
     * getting the pairs and printing the result based on the data provided by user.
     */
    do {
        printf("\nPlease provide the root and the node you want to check: \n\n");
        s = scanf("%d %d", &u, &v);
        res = path(AdjacencyMatrix, u, v);

        if (res == TRUE) {
            green();
            printf("TRUE. There is a path from %d to %d\n", u, v);
            reset();
        } else if (u < -1 || v < -1 || u >= N || v >= N) {
            red();
            printf("FALSE. One of the values is exceeding the limit.\n");
            reset();
        } else if (u == -1 && v == -1) {
            grey();
            printf("Program terminated by -1,-1 values.\n");
            reset();
            break;
        } else {
            yellow();
            printf("FALSE. There isn't a path from %d to %d\n", u, v);
            reset();
        }
    } while ((s != EOF));
    reset();
    return 0;
}

