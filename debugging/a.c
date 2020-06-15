#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 100

typedef struct photos {
    int serialNumber;
    char *ofiiceName;
    char *photographerName;
    int numOfPhotos;
    int wasPublished;
    struct photos *next;
} photos;

void fileReader(FILE *fp, photos **head);

void addNode(photos *node, photos **head);

void freeList(photos **head);

void fileReader(FILE *fp, photos **head) {
    char buffer[BUFSIZE];
    char *p;
    photos *tmpPtr = NULL;

    while (!feof(fp)) {
        if (!fgets(buffer, BUFSIZE, fp))
            continue;
        tmpPtr = *head;
        p = strtok(buffer, ",");
        tmpPtr->serialNumber = atoi(p);
        p = strtok(NULL, ",");
        strcpy(tmpPtr->ofiiceName, p);
        p = strtok(NULL, ",");
        strcpy(tmpPtr->photographerName, p);
        p = strtok(NULL, ",");
        tmpPtr->numOfPhotos = atoi(p);
        p = strtok(NULL, ",");
        tmpPtr->wasPublished = atoi(p);
        addNode(tmpPtr, head);
    }
}

void addNode(photos *node, photos **head) {
    photos *curr = *head;
    while (curr->next) {
        curr = curr->next;
        curr->next = node;
        node->next = NULL;
    }
}

void freeList(photos **head) {
    photos *curr = *head;
    while (curr) {
        photos *next = curr->next;
        free(curr);
        curr = next;
    }

}

void findSmallest(int head){

}
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "missing file name\n");
        exit(1);
    }

    if (argc == 2) {
        photos *head = (photos *) malloc(sizeof(photos));
        FILE *fp;
        fp= fopen(argv[1],"r");
        fileReader(fp,&head);
        fclose(fp);
        freeList(&head);
    }
    return 0;
}
