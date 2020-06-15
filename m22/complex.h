
#ifndef MMN_22V1_1_COMPLEX_H
#define MMN_22V1_1_COMPLEX_H
/*
 * =========================================================================================
 * name        : mmn_22/complex.h
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : Header file for complex.c
 * =========================================================================================
 */

/*defining the compleX structure */
typedef struct compleX {
    double a, b;
} compleX;

/*extern global variables*/
extern compleX a, b, c, d, e, f;


/*===================prototype declerations for the complex.c functions=================*/

/*
 *sets compleX variable with the params:
 * pointer to compleX, double a, double b
 */
void read_comp(compleX *cp, double a, double b);

/*
 *prints compleX variable. params: pointer to complex
 */
void print_comp(compleX *cp);

/*
 * Sums two complex numbers and print the the result.
 * params: two pointers
 * */
void add_comp(compleX *cpA, compleX *cpB);

/*
 * Subtracting two complex numbers and print the result.
 * params: two pointers, one for each variable.
 */
void sub_comp(compleX *cpA, compleX *cpB);

/*
 * Calculates the multiplication between complex number and real number
 * params: pointer to compleX and real number
 * */
void mult_comp_real(compleX *cp, double scalar);


/*
 * Calculates the multiplication between two complex number and imaginary number
 * params: pointer to compleX and img number.
 * */
void mult_comp_img(compleX *cp, double img);

/*
 * Calculates the multiplication between two complex numbers
 * params: two pointers, one for each variable.
 * */
void mult_comp_comp(compleX *cpA, compleX *cpB);


/*
 * Calculates the absolute value of complex number
 * params: pointer to compleX.
 * */

void abs_comp(compleX *cp);


#endif /*MMN_22V1_1_COMPLEX_H*/
