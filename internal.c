#include "functions.h"

void showvar(var **head, char *mode, char *file){
    char store[999] = {};
    char specify[999] ={};
    get_txt(specify);
    if (strcmp(mode, "default") == 0) {

        if (strlen(specify) != 0) {
            if (find_list(specify, store, *head)) {
                printf("%s=%s\n", specify, store);
            } else {
                printf("Error: not a shell variable\n");
            }
        }
        else {
            find_all_list(*head);
        }

    }
    else if(strcmp(mode, "input")==0){
        char *line = NULL;
        size_t len;

        FILE *fPointer;

        fPointer = fopen(file, "r");

        if (fPointer == NULL) {
            printf("file does not exist\n");
            return;
        }

        fseek(fPointer, 0, SEEK_END);
        if (ftell(fPointer) == 0){ // if file is empty
            find_all_list(*head);
        }
        else{
            fseek(fPointer, 0, SEEK_SET);

            while (getline(&line, &len, fPointer) != -1) {
                strtok(line, "\n");
                if (find_list(line, store, *head)) {
                    printf("%s=%s\n", line, store);
                } else {
                    printf("Error: not a shell variable\n");
                }
            }

            if (line) {
                free(line);
            }
        }

    }
    else {

        if (strcmp(file, "store.txt") == 0) {
            printf("Error: Cannot access file\n");
            return;
        }

        if (strlen(specify) != 0) {
            if (find_list(specify, store, *head)) {
                FILE *fPointer;
                if (strcmp(mode, "redirect") == 0) {
                    fPointer = fopen(file, "w");
                }
                else {
                    fPointer = fopen(file, "a");
                }
                fprintf(fPointer, "%s=%s\n", specify, store);

                fclose(fPointer);
            }
            else {
               printf("Error: not a shell variable\n");
            }

        }
        else {

            find_all_list_r(*head, file, mode);

        }
    }
}

void echo(char *mode, char *file){
    char store[999] = {};
    get_txt(store);

    if (strcmp(mode, "default") == 0){
            printf("%s\n", store);
    }
    else {
        FILE *fPointer;

        if (strcmp(file, "store.txt") == 0){
            printf("Error: Cannot access file\n");
            return;
        }

        if (strcmp(mode, "redirect") == 0) {

            fPointer = fopen(file, "w");

            fprintf(fPointer, "%s\n", store);

        }
        else if (strcmp(mode, "a_redirect") == 0) {

            fPointer = fopen(file, "a");

            fprintf(fPointer, "%s\n", store);

        }
        else {
            char *line = NULL;
            size_t len;
            fPointer = fopen(file, "r");

            if (fPointer == NULL) {
                printf("Error: file does not exist\n");
                return;
            }

            while (getline(&line, &len, fPointer) != -1) {
                printf("%s", line);
            }

            if (line) {
                free(line);
            }

        }
        fclose(fPointer);
    }
}

void export(var **head){

    char store[999] = {};
    char specify[999] = {};
    get_txt(specify);

    if(strlen(specify)!=0){
        if(find_list(specify, store, *head)){
            setenv(specify,store,1);
        }
        else{
            printf("Error: not a shell variable\n");
        }
    }

}

void push(struct Stacks **shead){
    char store[999] = {};

    get_txt(store);
    DIR* dir = opendir(store);
    if(dir) { //exists
        pushd(store, shead);
    }
    else if (ENOENT==errno)
    {
        printf("Error: Directory does not exit\n");
    }
    else{
        printf("Error: Failed to check directory\n");
    }
}


void cd(var **head, struct Stacks *shead){
    char store[999] = {};
    get_txt(store);

    if(chdir(store) == 0){
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        update_var(*head, "CWD", cwd);

        update_bottom(shead);
    }
    else if(strlen(store)==0){
        printf(" ");
    }
    else {
        printf("Error : No such file or directory\n");
    }

}

void unset(var **head){

    char *store;
    char specify[999] ={};
    get_txt(specify);

    if(strlen(specify)!=0) {
        store = getenv(specify);
        if (store!= NULL) {
            unsetenv(specify);
        }
    }
    delete(head,specify);

}

void showenv(char *mode, char *file){
    char *store;
    char specify[999] ={};
    get_txt(specify);
    if (strcmp(mode, "default") == 0) {

        if(strlen(specify)!=0){
            store = getenv(specify);
            if(store!= NULL){
                printf("%s=%s\n", specify, store);
            }
            else{
                printf("Error: not a shell variable\n");
            }
        }
        else {
            find_all_env();
        }

    }
    else if(strcmp(mode, "input")==0){
        char *line = NULL;
        size_t len;

        FILE *fPointer;

        fPointer = fopen(file, "r");

        if (fPointer == NULL) {
            printf("file does not exist\n");
            return;
        }

        fseek(fPointer, 0, SEEK_END);
        if (ftell(fPointer) == 0){ // if file is empty
            find_all_env();
        }
        else{
            fseek(fPointer, 0, SEEK_SET);

            while (getline(&line, &len, fPointer) != -1) {
                strtok(line, "\n");
                    store = getenv(line);
                    if(store!= NULL){
                        printf("%s=%s\n", line, store);
                    }
                    else{
                        printf("Error: not a shell variable\n");
                    }
                }

            if (line) {
                free(line);
            }
        }

    }
    else {

        if (strcmp(file, "store.txt") == 0) {
            printf("Cannot access file\n");
            return;
        }

        if (strlen(specify) != 0) {
            store = getenv(specify);
            if (store!= NULL){
                FILE *fPointer;

                if (strcmp(mode, "redirect") == 0) {
                    fPointer = fopen(file, "w");
                }
                else {
                    fPointer = fopen(file, "a");
                }
                fprintf(fPointer, "%s=%s\n", specify, store);

                fclose(fPointer);
            }
            else {
                printf("Error: not a shell variable\n");
            }

        }
        else {

            find_all_env_r(file, mode);

        }
    }

}

void source(int *int_cmd,var **head, struct Stacks **shead, var *newVar){
    char specify[999] ={};
    char tmp[MAX_LENGTH] = { 0 };
    char file[MAX_LENGTH];
    get_txt(specify);
    char *ptr = file;

    FILE *fPointer; //clears file
    char *line = NULL;
    size_t len;

    fPointer = fopen(specify, "r");
    if(fPointer == NULL){
        printf("file does not exist\n");
        return;
    }
    while (getline(&line, &len, fPointer) != -1){
        strtok(line, "\n");
        run_int(int_cmd, head, shead, newVar, line, ptr);
        split_space_not_quote(line, int_cmd, tmp, head, newVar, ptr);
    }
    run_int(int_cmd, head, shead, newVar, line, ptr);

    fclose(fPointer);
    if(line) {
        free(line);
    }
}

void run_int(int *int_cmd, var **head, struct Stacks **shead, var *newVar, const char *s, char *file){
    char mode[MAX_LENGTH];

    if(int_cmd[1] == 1){
        strcpy(mode, "redirect");
    }
    else if(int_cmd[1] == 2) {
        strcpy(mode, "a_redirect");
    }
    else if(int_cmd[1] == 3) {
        strcpy(mode, "input");
    }
    else{
        strcpy(mode, "default");
    }

    if(int_cmd[0] == 1){
        echo(mode, file);
    }
    else if(int_cmd[0] == 2){
        showvar(head, mode, file);
    }
    else if(int_cmd[0] == 3){
        cd(head, *shead);
    }
    else if(int_cmd[0] == 5){
        export(head);
    }
    else if(int_cmd[0] == 7){
        unset(head);
    }
    else if(int_cmd[0] == 8){
        showenv(mode , file);
    }
    else if(int_cmd[0] == 9){
        push(shead);
    }
    else if(int_cmd[0] == 10){
        popd(shead);
    }
    else if(int_cmd[0] == 11){
        print_stack(*shead, mode, file);
    }
    else if(int_cmd[0] == 12){
        peek(*shead);
    }
    else if(int_cmd[0] == 13){
        int_cmd[0] = 0;
        source(int_cmd,head,shead, newVar);
    }
    else if(int_cmd[0] == 14){
        external(s);
    }

    int_cmd[0] = 0, int_cmd[1] = 0;


    FILE *fPointer; //clears file
    fPointer = fopen("store.txt", "w");
    fclose(fPointer);

}

bool check_internal(char *temp, bool *first, int *int_cmd, char *mode)
{
   if(strcmp(mode, "redirect")==0){
       int_cmd[1] = 1;
       return true;
   }
   else if(strcmp(mode, "a_redirect")==0){
       int_cmd[1] = 2;
       return true;
   }
   else if(strcmp(mode, "input")==0){
       int_cmd[1] = 3;
       return true;
   }

    if(*first==true) {

        if ((strcmp(temp, "echo") == 0)) {
            int_cmd[0] = 1;
        }
        else if((strcmp(temp, "exit") == 0)){
            exit(0);
        }
        else if((strcmp(temp, "showvar") == 0)){
            int_cmd[0] = 2;
        }
        else if((strcmp(temp, "cd") == 0)){
            int_cmd[0]= 3;
        }
        else if((strcmp(temp, "export") == 0)){
            int_cmd[0] = 5;
        }
        else if((strcmp(temp, "unset") == 0)){
            int_cmd[0]= 7;
        }
        else if((strcmp(temp, "showenv") == 0)){
            int_cmd[0] = 8;
        }
        else if((strcmp(temp, "pushd") == 0)){
            int_cmd[0]= 9;
        }
        else if((strcmp(temp, "popd") == 0)){
            int_cmd[0] = 10;
        }
        else if((strcmp(temp, "dirs") == 0)){
            int_cmd[0] = 11;

        }
        else if((strcmp(temp, "pwd") == 0)){
            int_cmd[0] = 12;
        }
        else if((strcmp(temp, "source") == 0)){
            int_cmd[0] = 13;
        }
        else{ // external
            int_cmd[0] = 14;
        }

        *first = false;
        return true;

    }

    return false;
}