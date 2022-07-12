#include "functions.h"

struct variable_key_value *add_node(struct variable_key_value *list, struct variable_key_value *Val){
    struct variable_key_value *prev, *next;
    if (!list) {
        list = Val;
    }
    else {
        prev = NULL;
        next = list;
        while (next && strcmp(Val->key, next->key)>0) {
            prev = next;
            next = next->next;
        }
        if (!next) {
            prev->next = Val;
        }
        else {
            if (prev) {
                Val->next = prev->next;
                prev->next = Val;
            }
            else {
                Val->next = list;
                list = Val;
            }
        }
    }
    return list;
}

void insert(char *keyc, char *valuec, var **head, var *newVar){

    if(!update_var(*head,keyc,valuec)){
        newVar = (struct variable_key_value*)malloc(sizeof(struct variable_key_value));

        strcpy(newVar->key, keyc);
        strcpy(newVar->value, valuec);

        newVar->next = NULL;

        *head = add_node(*head, newVar);
    }

}

void delete(struct variable_key_value **head, char  *keyc){

    struct variable_key_value *temp = *head, *prev;

    if(temp!= NULL && strcmp(temp->key, keyc)==0){ //the node is the one to be deleted
        *head = temp->next;
        free(temp);
        return;
    }

    while(temp!=NULL && strcmp(temp->key, keyc)!=0){ //search for key to be deleted
       prev = temp;
       temp = temp->next;
    }

    if(temp==NULL){ //not found
        return;
    }

    prev->next = temp->next; //unlink

    free(temp);

}

void free_list(var *head) {
    struct variable_key_value *prev;
    struct variable_key_value *cur = head;
    while (cur) {
        prev = cur;
        cur = prev->next;
        free(prev);
    }
}

bool find_list(char *key, char *temp, var *head) {
    bool check=false;

    while (head) {
        if(strcmp(key, head->key)==0) {
            strcpy(temp, head->value);
            check=true;
        }
        head = head->next;
    }
    return check;
}

void find_all_list(struct variable_key_value *head) {
    while (head) {

        printf("%s=%s\n", head->key, head->value);

        head = head->next;
    }
}

void find_all_list_r(struct variable_key_value *head,char *file, char *mode) {
    FILE *fPointer;

    if (strcmp(mode, "redirect") == 0) {
        fPointer = fopen(file, "w"); //first remove previous contents
        fclose(fPointer);
        fPointer = fopen(file, "a");

        while (head) {
            fprintf(fPointer, "%s=%s\n", head->key, head->value);

            head = head->next;

        }
        fclose(fPointer);
    }
    else {
        fPointer = fopen(file, "a");

        while (head) {
            fprintf(fPointer, "%s=%s\n", head->key, head->value);

            head = head->next;

        }
        fclose(fPointer);
    }
}


void find_all_env() {

  char **s = environ;
    for(; *s; s++){
        printf("%s\n", *s);
    }

}

void find_all_env_r(char *file, char *mode){
    FILE *fPointer;

    if (strcmp(mode, "redirect") == 0) {
        fPointer = fopen(file, "w"); //first remove previous contents
        fclose(fPointer);
        fPointer = fopen(file, "a");

        char **s = environ;
        for(; *s; s++){
            fprintf(fPointer, "%s\n", *s);
        }

        fclose(fPointer);
    }
    else {
        fPointer = fopen(file, "a");

        char **s = environ;
        for(; *s; s++){
            fprintf(fPointer, "%s\n", *s);
        }

        fclose(fPointer);
    }
}

bool update_var(var *head, char *new, char *valuec){

    if(head==NULL){
        printf("System Error\n");
        return false;
    }

    while (head) {
        if(strcmp(new, head->key)==0){
            strcpy(head->value, valuec);
            return true;
        }
        head = head->next;
    }

    return false;
}