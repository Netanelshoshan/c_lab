#include <stdio.h>

int main(){

    int v[] = {-9,6,3,0,5};
    int *p,n;

    p = v + 2;
    n = --*--p;
    (p)++;
    printf("%d ",*p);
    printf("%d %d %d\n",n,*p,p[1]);
}