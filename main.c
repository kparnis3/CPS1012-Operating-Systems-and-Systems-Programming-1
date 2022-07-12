#include "functions.h"

int main(int argc, char **argv){
    setvbuf(stdout, NULL, _IONBF, 0);

    char line[MAX_LENGTH], file[MAX_LENGTH], tmp[MAX_LENGTH] = { 0 }, prompt[MAX_PROMT] = { "PROMPT>" };
    int int_cmd[2] = {0,0};
    char *ptr = file, *ptr3 = prompt;
    int *ptr2 = int_cmd;
    var *head, *newVar;
    head= NULL, newVar = NULL;

    variables(&head, newVar, *argv, ptr3); //sets variables
    struct Stacks *root = ini_stk(); //sets the bottom of stack

    do {

        run_int(ptr2, &head, &root, newVar, line, ptr); //runs internal commands and clears text file
        memset(file, 0, sizeof file); //empty file
        find_list("PROMPT", prompt, head); //check if prompt changed
        printf("%s ", prompt);
        sgets(line, sizeof(line));

        split_space_not_quote(line, ptr2, tmp, &head, newVar, ptr);

    }
    while(strcmp(line,"exit")!=0);

    free_stack(&root);
    free_list(head);
    return 0;
}

