/*
 * =========================================================================================
 * name        : mmn_22/complex.c
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : contains all the calculating functions.
 * =========================================================================================
 */

#include "complex.h"
#include "common.h"


/*Function for reading input from the user*/
void read_comp(compleX *cp, double a, double b) {
    (*cp).a = a;
    (*cp).b = b;
}


/* Prints the variable given */
void print_comp(compleX *cp) {
    printf("%s\n*SYSTEM: Print Complex Number : ", LINE);
    printf("%.2f + (%.2f)i\n", (*cp).a, (*cp).b);
    printf("%s\n", LINE);
}

/* Sums two complex numbers (aka variables)*/
void add_comp(compleX *cpA, compleX *cpB) {
    compleX temp;
    temp.a = (*cpA).a + (*cpB).a;
    temp.b = (*cpA).b + (*cpB).b;
    printf("\nThe result of the addition:\n");
    print_comp(&temp);
}

/* Calculate the subtraction of two complex numbers*/
void sub_comp(compleX *cpA, compleX *cpB) {
    compleX temp;
    temp.a = (*cpA).a - (*cpB).a;
    temp.b = (*cpA).b - (*cpB).b;
    printf("\nThe result of the subtraction:\n");
    print_comp(&temp);
}

/* Calculates the multiplication between complex number and real number*/
void mult_comp_real(compleX *cp, double real) {
    compleX temp;
    temp.a = (*cp).a * real;
    temp.b = (*cp).b * real;
    printf("\nThe result of the multiplication with real:\n");
    print_comp(&temp);

}


/* Calculates the multiplication between two complex number and imaginary number */
void mult_comp_img(compleX *cp, double img) {
    compleX temp;
    temp.a = (*cp).a * img;
    temp.b = -(*cp).b * img;
    printf("\nThe result of the multiplication with img:\n");
    if (temp.a < 0){
    	printf("%s\n*SYSTEM: Print Complex Number : ", LINE);
        printf("%.2f + (%.2f)i\n", temp.b, temp.a);
    	printf("%s\n", LINE);
    }else{
    	printf("%s\n*SYSTEM: Print Complex Number : ", LINE);
        printf("%.2f + %.2fi\n", temp.b, temp.a);
    	printf("%s\n", LINE);
    }
}

/* Calculates the multiplication between two complex numbers */
void mult_comp_comp(compleX *cpA, compleX *cpB) {
    compleX temp;

    temp.a = ((*cpA).a * (*cpB).a) - ((*cpA).b * (*cpB).b);
    temp.b = ((*cpA).a * (*cpB).b) + ((*cpA).b * (*cpB).a);

    printf("\nThe result of the multiplication of two complex numbers:\n");
    print_comp(&temp);
}


/* Calculates the absolute value of complex number */
void abs_comp(compleX *cp) {
    double abs = sqrt((pow((*cp).a, 2)) + (pow((*cp).b, 2)));
    printf("\nThe Absolute value of the variable is: %.2f\n",abs);
}
