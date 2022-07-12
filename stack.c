#include "functions.h"

struct Stacks *ini_stk(){

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    struct Stacks* root = (struct Stacks*) malloc(sizeof(struct Stacks));
    strcpy(root->dir, cwd);

    return root;
}

void pushd(char *temp, struct Stacks** stack)
{
    struct Stacks* stc = (struct Stacks*)malloc(sizeof(struct Stacks));
    strcpy(stc->dir, temp);
    stc->next = *stack;
    (*stack) = stc;

    print_stack(*stack, "default", NULL);
}

void popd(struct Stacks** stack)
{
    if(count(*stack) > 1){
        struct Stacks* temp = *stack;
        *stack = (*stack) -> next;
        free(temp);

        print_stack(*stack, "default", NULL);
    }
    else
    {
        printf("Error: can't popd first item\n");
    }

}

void update_bottom(struct Stacks* stack)
{
   while (stack-> next!= NULL) {
      stack = stack->next;
   }

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    strcpy(stack->dir, cwd);


}

void peek(struct Stacks* stack){
    if(stack!=NULL){
        printf("%s\n", stack ->dir);
    }
    else{
        printf("Error: Stack is empty\n");
    }
}

void free_stack(struct Stacks** stack) {

    struct Stacks *prev;
    struct Stacks *cur = *stack;

    while (*stack!=NULL) {
        *stack = (*stack) -> next;
        prev = cur;
        cur = prev->next;
        free(prev);
    }
}

int count(struct Stacks* stack){
    int count = 0;
    struct Stacks* curr = stack;
    while(curr !=NULL){
        count++;
        curr = curr->next;
    }

    return count;
}


void print_stack(struct Stacks* stack, char *mode, char *file) {
    if (strcmp(mode, "default") == 0) {
        while (stack) {

            printf("%s ", stack->dir);

            stack = stack->next;
        }
        printf("\n");
    }

    else {
        FILE *fPointer;

        if (strcmp(mode, "redirect") == 0) {
            fPointer = fopen(file, "w");
        }
        else {
            fPointer = fopen(file, "a");
        }
            while (stack) {

                fprintf(fPointer, "%s ", stack->dir);

                stack = stack->next;
            }

            fprintf(fPointer, "\n");

            fclose(fPointer);
        }
    }
