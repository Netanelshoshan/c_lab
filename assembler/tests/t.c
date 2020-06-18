#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Code line structure for file parsing */
typedef struct file_input
{
    struct
    {
        char *content;   /* line content */
        int moreParsing; /* if the line need's more than one parsing (used in 2nd stage) - (for labels) */
        int isDone;      /* marking line as done only if the line doesn't require more the one parsing. - (for whole line) */
    } line;
    int lineNum;  /* line number for error handling and more */
    char *srcOpr; /* if src op is a label, it will hold it, otherwise it's a reg and "instructionLine" will hold it */
    char *dstOpr; /* ^same goes here^ */
} File_input;

int main()
{
    File_input *f;
    f = malloc(sizeof(char) * 80);
    f[0].line.content = malloc(sizeof(char) * 80);
    f->line.content = ".externalio   t";
    File_input *s;
    s = malloc(sizeof(char) * 80);
    s[0].line.content = malloc(sizeof(char) * 80);
    s->line.content = ".extern t";

    if (strncmp(f->line.content, ".extern", sizeof(".extern") - 1) == 0)
    {
                printf("first:\t%s\n", f->line.content);

        /*.extern handling */
        /*advance the pointer beyond deceleration and parse the extern label */
        f->line.content += (sizeof(".extern") - 1);
                printf("after +=:\t%s\n", f->line.content);

        goto isValid;
                printf("after isspace:\t%s\n", f->line.content);


    }
    else
    {
    cmd_err:
        printf("ERR\n");
    }

isValid:
    if (!isspace(*(input->line).content))
    {
        input->line.content -= (sizeof(".extern") - 1);
        symbol = strtok(input->line.content, "\n");
        input->line.isDone = 1;
        goto cmd_err;
        return;
    }

    if (strncmp(s->line.content, ".extern", sizeof(".extern") - 1) == 0)
    {
        printf("first:\t%s\n", s->line.content);
        s->line.content += (sizeof(".extern") - 1);
        printf("after +=:\t%s\n", s->line.content);

        if (!isspace(*(s->line).content))
            s->line.content -= (sizeof(".extern") - 1);

        printf("after isspace:\t%s\n", s->line.content);
    }
    printf("none\n");
    return 0;
}