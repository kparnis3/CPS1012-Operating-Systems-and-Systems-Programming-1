#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <sys/wait.h>

#define MAX_LENGTH 999
#define MAX_PROMT 20
#define MAX_KEY 20

#define PATH_AMOUNT 7

extern char **environ;

typedef struct variable_key_value{
    char key[MAX_LENGTH];
    char value[MAX_LENGTH];
    struct variable_key_value *next;

} var;

struct Stacks{
    char dir[MAX_LENGTH];
    struct Stacks* next;
};

struct Stacks *ini_stk();

// MAIN FUNCTIONS

void variables(var **head, var *newVar, char *argv, char *prompt);

void printstring(const char *frm, const char *to, char *temp, int quoted, int *int_cmd, var **head, var *newVar, bool *isfirst, char *file);

void split_space_not_quote( const char *s, int *int_cmd, char *temp, var **head, var *newVar, char *file);

char *sgets(char *str, int size);

// STACK FUNCTIONS

void pushd(char *temp, struct Stacks** stack);

void popd(struct Stacks  **stack);

void print_stack(struct Stacks* stack, char *mode, char *file);

void free_stack(struct Stacks** stack);

void peek(struct Stacks* stack);

int count(struct Stacks* stack);

void update_bottom(struct Stacks* stack);

// LINKED LIST FUNCTIONS

struct variable_key_value *add_node(struct variable_key_value *list, struct variable_key_value *Val);

bool update_var(var *head, char *new, char *valuec);

void insert(char *keyc, char *valuec, var **head, var *newVar);

void delete(struct variable_key_value **head, char  *keyc);

void free_list(var *head);

bool find_list(char *key, char *temp, var *head);

void find_all_list(struct variable_key_value *head);

void find_all_list_r(struct variable_key_value *head, char *file, char *mode);

void find_all_env_r(char *file, char *mode);

void find_all_env();

// INTERNAL FUNCTIONS

void run_int(int *int_cmd, var **head, struct Stacks **shead, var *newVar, const char *s,char *file);

bool check_internal(char *temp, bool *first,int *int_cmd, char *mode);

char *get_txt(char *str);

void echo(char *mode, char *file);

void unset(var **head);

void showvar(var **head, char *mode, char *file);

void showenv(char *mode, char *file);

void source(int *int_cmd, var **head, struct Stacks **shead, var *newVar);

void export(var **head);

void push(struct Stacks **shead);

void cd(var **head, struct Stacks *shead);

// EXTERNAL FUNCTION

int external(const char *s);










