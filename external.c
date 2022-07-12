#include "functions.h"

int external(const char *s){
    char store[MAX_LENGTH] = {};
    char command[MAX_KEY];

    get_txt(store);
    int i = 0;
    while(*s){ //gets the command
      if(*s == ' '){
          break;
      }
      command[i] = *s;
      i++;
      s++;
    }

    pid_t pid_result = fork();
    if(pid_result == -1){
        printf("Error: command not found\n");
        return EXIT_FAILURE;
    }

    if(pid_result > 0){
        if(wait(NULL) == -1){
            printf("Error: Wait failed\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    //Child Process
    if(strlen(store)==0) { //command with no arguments
        char *const args[] = {command, NULL, NULL};
        if(execvp(args[0],args) == -1){
            printf("Error: No such file or directory\n");
            return EXIT_FAILURE;
        }

    }
    else{
            char *const args[] = {command,store, NULL}; //command with arguments
            if(execvp(args[0],args) == -1){
                printf("Error: No such file or directory\n");
                return EXIT_FAILURE;
            }
        }


    return EXIT_SUCCESS;
}