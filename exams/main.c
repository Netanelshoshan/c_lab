#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>

#define arr_sz 100
int arr[arr_sz] = {1, 2, 3, 4, 5, 6, 7, 0};

typedef struct members {
    int candidate_id;
    int exp_per_cand;
    int expirence;
} members;

void find_candidate(members *list) {

}

int main3(int argc, char **argv) {
    char buf[100];
    FILE *fp;
    if (argc < 2) {
        fprintf(stderr, "missing file name\n");
        exit(1);
    }
    if (argc == 2) {
        fp = fopen(argv[1],"r");
        members *head = (members *) malloc(sizeof(members));
        fread(buf,100,1,fp);
        creat()
    }

}




int arr_cnt(int n, int m) {
    int *p;
    int cnt = 0;/*i=0;*/

    p = &arr[n];
    while (p <= &arr[m]) {
        cnt += *p;
        p++;
    }
    /*free(p);*/
    return cnt;
}

void a(int x) {
    if (x > 0) {
        a(x - 1);
        printf("%d\n", x);
    }
}


int main() {
    char *join(char *, char *);
    printf("%s\n", join("USA ", "IL"));
    return 0;
}


char *join(char *s1, char *s2) {
    char *src = (char *) malloc(strlen(s2) + 1);
    char *dst = (char *) malloc(strlen(s1) + 1);
    if (!src || !dst) {
        printf("malloc err");
        exit(1);
    }
    strcpy(dst, s1);
    strcpy(src, s2);
    return strcat(dst, src);
}

/*

int main(void) {
    int sum;
    */
/*sum = arr_cnt(2, 11);*//*

    a(99);
    return 0;
}
*/
