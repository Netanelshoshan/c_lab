/*
 * =========================================================================================
 * name        : mmn_22/mycomp.c
 * author      : Netanel Shoshan, ID 316393321
 * email       : netanelShoshan@outlook.com
 * description : contains main function, The structure of the
 * complex variables and cmd commands, error handling and validation functions.
 * =============================================================================================
 */

#include "complex.h"
#include "common.h"

/* Error messages array */
char *errorMsgs[] = {
        "*ERROR: Command %s does not exist\n",
        "*ERROR: Invalid %s\n",
        "*ERROR: Command too %s!!\n",
        "*ERROR: Variable %s does not exist!\n",
        "*ERROR: Missing %s\n",
        "*ERROR: Extra text after the end of command.\n",
        "*ERROR: Missing comma.\n",
        "*INFORM: Please make sure to provide the correct input.\n For more info, enter [help].\n",
        "*INFORM: Reached the end of file provided. exiting...\n"
};

/* System messages array */
char *systemMsgs[] = {
        "Please enter valid function name and suitable parameters:\nUsage [function_name] [variable],[parameters]",
        "Available variables: A -> E\nAvailable functions:\nread_comp           [complex variable],[parameter],[parameter]\nprint_comp          [complex variable]\nadd_comp            [complex variable],[complex variable]\nsub_comp            [complex variable],[complex variable]\nmult_comp_real      [complex variable],[scalar]\nmult_comp_img       [complex variable],[scalar]\nmult_comp_comp      [complex variable],[complex variable]\nabs_comp            [complex variable]\nstop                [none]",
        "*INFORM: Program halted!\n"
};

/* Defining compleX names and initializing structure definition */
compleX a, b, c, d, e, f;

struct {
    char *name;
    compleX *var;
} storage[] = {
        {"A", &a},
        {"B", &b},
        {"C", &c},
        {"D", &d},
        {"E", &e},
        {"F", &f},
        {"0", NULL}
};

/* Initializing the structure for the commands available for the user.*/
struct {
    char *name;

    void (*func)();

    int funcParam;
} cmd[] = {
        {"read_comp",      read_comp,      ARGS},
        {"print_comp",     print_comp,     NONE},
        {"add_comp",       add_comp,       VAR},
        {"sub_comp",       sub_comp,       VAR},
        {"mult_comp_real", mult_comp_real, SCALAR},
        {"mult_comp_img",  mult_comp_img,  SCALAR},
        {"mult_comp_comp", mult_comp_comp, VAR},
        {"abs_comp",       abs_comp,       NONE},
        {"not_valid", NULL,                NONE}
};

/* Main function rather simple.
 * it calls the startUserInterface() which does the rest.
 * */
int main() {
    startUserInterface();
    return 0;
}

/*
 * This function acts like a gateway into the program.
 * the function is resposible for:
 * 1. providing ui.
 * 2. handling the data provided by the user.(through a file or one command at a time.)
 * 3. input validation.
 * 4. sorting the input and making sure the calculating functions will get the correct input.
 *
*/
void startUserInterface() {

    /*Initializing and declaring states,variables,indexes etc..*/
    char command[MAX_BUFFER], ch;
    int state = WAIT_FOR_CMD_NAME, paramState = NONE, funcIndex = 0, varIndex = 0;
    int i, j = 0;
    char *cmdName = "", *varName = "", *rest = "";

    /*========================welcoming message=========================*/
    printf("%s\n", LINE);
    printf("\t%s, %s\n", PROG_NAME, VERSION);
    printf("%s\n", LINE);

    printf("%s\n\n", systemMsgs[START]);
    printf("%s\n%s\n", systemMsgs[HELP], LINE);

    /*=========================input-handling==========================*/
    FOREVER {

        state = WAIT_FOR_CMD_NAME;
        clearCommandString(command); /*empty the buffer*/
        printf("\n->");

        /*Reading the input from the user and assigns it to [command] array */
        for (i = 0; j != ENTER_KEY && j != EOF; i++) {
            j = getchar();
            if (j == ENTER_KEY) break;
            else if (j == EOF) {
                errorHandler("", EOF_);
                exit(1);
            } else if (i > MAX_BUFFER) {
                errorHandler("long", CMD);
                state = ERROR;
                clearBuffer();
                break;
            } else {
                ch = j;
                command[i] = ch;
            }

        }
        /*if stop was provided */
        if (strcmp(command, "stop") == 0) {
            printf("\n%s\n", LINE);
            printf("%s", systemMsgs[HALT]);
            printf("%s\n", LINE);
            exit(0);
        }
        /*prints help page*/
        if (strcmp(command, "help") == 0) {
            printf("%s", systemMsgs[HELP]);
            state = ERROR;
        }

        /*if the command it too short */
        if (i < MIN_COMMAND) {
            errorHandler("short", CMD);
            state = ERROR;
        }
        j = 0;


        /*==========================command-checking=========================*/
        /*
         * The algrithem is rather simple:
         * - Take the array, cut it , clean in and assign it to variables based on what's
         *   left.
         * - decisions made by length checking.
         */

        if (state == WAIT_FOR_CMD_NAME) {

            int commandLength, funcLength, varLength;
            commandLength = strlen(command);

            cmdName = strtok(command, " ");/* gets the function name*/
            funcLength = strlen(cmdName);

            if (commandLength - funcLength >= 2 && strcmp(command, "stop") != 0) {

                /* Gets the complex var (A,B,C..) */
                varName = strtok(NULL, ",");
                varLength = strlen(varName);

                /*complex var is digit*/
                if (isdigit(*(varName) == 0)) {
                    state = ERROR;
                    errorHandler(varName, VAR_NOT_EXIST);
                    errorHandler("", ERROR_HELP);
                }
                    /*We have more length --> continue to cut.*/
                else if (commandLength - funcLength - varLength >= 2) {
                    rest = strtok(NULL, "");/*rest gets the "3.5,5.4" */

                    /*removing trailing spaces from rest and from the complex var*/
                    remove_spaces(rest);
                    remove_spaces(varName);
                }
                    /* handle cases like A B*/
                else if (isalpha(*(varName)) == 0 && varLength >= 2) {
                    state = ERROR;
                    errorHandler("", MISSING_COMM);
                } else if (strcmp(cmdName, "print_comp") == 0) {
                    printf(" ");
                } else if (strcmp(cmdName, "abs_comp") == 0) {
                    remove_spaces(varName);
                } else { /*low length --> not enough was given*/
                    state = ERROR;
                    errorHandler("parameters", LOW_ARGS);
                }
            } else {
                /*other stuff after the stop command*/
                if (strcmp(command, "stop") == 0) {
                    state = ERROR;
                    errorHandler(cmdName, EXTRA_AFTER);
                    errorHandler("", ERROR_HELP);
                } else {/*low length*/
                    state = ERROR;
                    errorHandler("parameters", LOW_ARGS);
                }
            }
        }
        /*===========================end of command-checking=======================*/

        /*==========================function finder===============================*/
        if (state != ERROR) {
            /*through over all command list*/
            for (i = 0; cmd[i].func != NULL; i++) {
                if (strcmp(cmdName, cmd[i].name) == 0)
                    break;
            }
            /* cmd wasn't found */
            if (cmd[i].func == NULL) {
                state = ERROR;
                errorHandler(cmdName, CMD_NOT_EXIST);
                errorHandler(cmdName, ERROR_HELP);
            } else {
                state = WAIT_FOR_VAR;
                funcIndex = i;
            }
        }
        /*=========================end-of function finder========================*/


        /*=========================complex variable checking========================*/
        if (state == WAIT_FOR_VAR) {

            /*through over all variable list*/
            for (i = 0; storage[i].var != NULL; i++) {
                if (strcmp(varName, storage[i].name) == 0)
                    break;
            }
            if (storage[i].var == NULL) {
                state = WAIT_FOR_CMD_NAME;
                errorHandler(varName, VAR_NOT_EXIST);
            } else if (cmd[funcIndex].funcParam == SCALAR) {
                if (rest[0] == '\x2d') {/*"-" handling*/
                    paramState = stateAsCmd(funcIndex);
                    state = WAIT_FOR_PARAMS;
                    varIndex = i;
                }else if (isalpha(rest[0])) { /*nan case*/
                    state = WAIT_FOR_CMD_NAME;
                    errorHandler("agrument after variable. Not a number.", INVALID_ARGS);
                } else {/*all good */
                    paramState = stateAsCmd(funcIndex); /*state checking*/
                    state = WAIT_FOR_PARAMS;
                    varIndex = i;
                }
            } else if (cmd[funcIndex].funcParam == NONE && strlen(rest) > 0) {/* if extra args was given*/
                state = WAIT_FOR_CMD_NAME;
                errorHandler("", EXTRA_AFTER);
            } else {/*all good*/
                paramState = stateAsCmd(funcIndex);
                state = WAIT_FOR_PARAMS;
                varIndex = i;
            }
        }
        /*=========================end of complex variable checking========================*/

        /*=========================function calling========================*/
        if (state == WAIT_FOR_PARAMS) {
            switch (paramState) {
                case NONE: {
                    /*call command function with current variable*/
                    cmd[funcIndex].func(storage[varIndex].var);
                    break;
                }/*end none case*/

                case ARGS: {
                    int restLength = strlen(rest);
                    float c1, c2;
                    /*IMPORTANT step in order to bypass SIGSEGV errors!*/
                    if (restLength <= MIN_ARGS) {
                        state = ERROR;
                        errorHandler("parameters", LOW_ARGS);
                        break;
                    }
                    /*If length validation wasn't made, and the user provided
                     * "4.5" instead of "4.5,5.5" --> SIGSEGV */
                    c1 = atof(strtok(rest, ","));
                    c2 = atof(strtok(NULL, ","));
                    cmd[funcIndex].func(storage[varIndex].var, c1, c2);
                    printf("Read to variable %s, two inputs: %.3f, %.3f\n", storage[varIndex].name, c1, c2);
                    break;
                }/*end args case*/

                case VAR: {
                    int len;
                    len = strlen(rest);

                    if (len > 1) {
                        errorHandler("multiple arguments after variable.", INVALID_ARGS);
                    } else {
                        for (i = 0; storage[i].var != NULL; i++) {
                            if (strcmp(rest, storage[i].name) == 0)
                                break;
                        }
                        if (storage[i].var == NULL) {
                            errorHandler(rest, VAR_NOT_EXIST);
                        } else
                            cmd[funcIndex].func(storage[varIndex].var, storage[i].var);
                    }

                    break;
                }/*end var case */

                case SCALAR: {
                    int len;
                    len = strlen(rest);
                    if (len > MAX_SCALAR) {
                        errorHandler("number of arguments after variable", INVALID_ARGS);
                    } else {
                        float num = atof(rest);
                        cmd[funcIndex].func(storage[varIndex].var, num);

                    }

                    break;
                }/*end scalar case*/


            }/*end switch statement*/

        }
        /*empting the strings*/
        varName = "";
        rest = "";
    }
}

/* removes spaces from given string.*/
void remove_spaces(char *s) {
    const char *str = s;
    do {
        while (*str == ' ') {
            ++str;
        }
    } while ((*s++ = *str++));
}

/*parameter nedded for the function ,whetherr its NONE,ARGS etc.. */
int stateAsCmd(int indexOfCmd) {
    return cmd[indexOfCmd].funcParam;
}

/*error handler*/
void errorHandler(char *command, int msgIndex) {
    printf("\n%s\n", LINE);
    printf(errorMsgs[msgIndex], command);
    printf("%s\n\n", LINE);
}

/*buffer cleaner*/
void clearBuffer() {
    int cb;
    cb = getchar();
    while (cb)
        if (cb == ENTER_KEY) {
            return;
        } else
            cb = getchar();
}
/*command array cleaning*/
void clearCommandString(char *command) {
    int k;

    for (k = 0; k < MAX_BUFFER; k++) {
        command[k] = 0;
    }
}
