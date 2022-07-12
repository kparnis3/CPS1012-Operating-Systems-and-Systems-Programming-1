#include "functions.h"

void printstring(const char *frm, const char *to, char *temp, int quoted, int *int_cmd, var **head, var *newVar, bool *isfirst,  char *file){
    char temp2[999] = {};
    char temp3[999] = {};
    int count = 0;

    FILE *fPointer;
    fPointer = fopen("store.txt", "a");

    if(temp[0] != 0){ //initiate key
        int k=0;
        while (frm < to) {
            temp3[count] = *frm++;
            (count)++;
        }

        for (int j = 0; j < (strlen(temp3)); j++){
            if(temp3[j]!= '\"' && temp3[j]!= '\\') {
                temp2[k] = temp3[j];
                k++;
            }

        }
        insert(temp, temp2, head, newVar);

        memset(temp, 0, MAX_LENGTH * (sizeof temp[0]));
        return;
    }

    if(quoted == 0) { //first command
        while (frm < to) {
            temp3[count] = *frm++;
            (count)++;
        }

      if(!check_internal(temp3, isfirst, int_cmd, "default")){
          int k =0;
          for (int j = 0; j < (strlen(temp3)); j++){
              if(temp3[j]!= '\"' && temp3[j]!= '\\') {
                  temp2[k] = temp3[j];
                  k++;
              }

          }
          fprintf(fPointer,"%s",temp2);
      }
    }

    if(quoted == 1){ //inside a quote
        while (frm < to) {
            temp2[count] = *frm++;
            (count)++;
        }
        int k = 0;
        for (int j = 0; j < (strlen(temp2)); j++){
            if(temp2[j]!= '\"' && temp2[j]!= '\\') {
                temp3[k] = temp2[j];
                k++;
            }

        }
        fprintf(fPointer,"%s",temp3);

    }

    if(quoted == 3) { //variables
        *frm++;
        memset(temp3, 0, sizeof(temp3));

        while (frm < to) {
            temp2[count] = *frm++;
            (count)++;
        }

        find_list(temp2, temp3, *head);
        if(temp3[0] == 0){
            return;
        }

        fprintf(fPointer,"%s", temp3);

    }

    if(quoted==4) { //key
        while (frm < to) {
            temp[count] = *frm++;
            (count)++;
        }
    }

    if(quoted==5) {
        *frm++;
        while (frm < to) {
            file[count] = *frm++;
            (count)++;
        }
        check_internal(file, isfirst, int_cmd, "redirect");
    }

    if(quoted==6) {
        *frm++;
        while (frm < to) {
            file[count] = *frm++;
            (count)++;
        }
        check_internal(file, isfirst, int_cmd, "a_redirect");
    }

    if(quoted==7) {
        *frm++;
        while (frm < to) {
            file[count] = *frm++;
            (count)++;
        }
        check_internal(file, isfirst, int_cmd, "input");
    }


    fclose(fPointer);
}



void split_space_not_quote( const char *s, int *int_cmd, char *temp, var **head, var *newVar, char *file){
    const char *start;
    int state = ' ';
    bool first = true;

    while (*s) {
        switch (state) {
            case '\n':
            case ' ': // Consuming spaces
                if (*s == '\"') {
                    start = s;
                    state = '\"';  // begin quote
                }else if(*s == '\\'){
                    state = 'e';
                }
                else if (*s == '$') { //variable expansion case
                    start = s;
                    state = 'Y';
                }
                else if (*s == '>'){ // redirect case
                    start = s;
                    state = 'S';
                }
                else if (*s == '<'){ // input case
                    start = s;
                    state = 's';
                }
                else if (*s != ' ') {
                    start = s;
                    state = 'T';
                }

                break;

            case 'T': // non-quoted text
                if (*s == ' ' || *s==';' || *s=='|') { //default
                    printstring(start, s, temp, 0, int_cmd , head, newVar, &first, file);
                    start = s;
                    state = ' ';
                }else if(*s == '='){ //variable assignment
                    printstring(start, s, temp, 4, int_cmd , head, newVar, &first, file);
                    start = s;
                    state = ' ';
                }
                else if(*s == '\\'){
                    state='e';
                }
                else if (*s == '\"') {
                    state = '\"'; // begin quote
                }
                else if (*s == '$') {
                    printstring(start, s, temp, 1,int_cmd ,head, newVar, &first,file);
                    start = s;
                    state = 'Y';
                }

                break;

            case 'V':
                if (*s == ' ') {
                    printstring(start, s, temp, 3,int_cmd, head, newVar, &first,file);
                    start = s;
                    state = '\"';
                }
                else if(*s == '\"'){
                    printstring(start, s, temp, 3,int_cmd , head, newVar, &first,file);
                    start = s;
                    state = ' ';
                }
                break;

            case 'S': // redirect case
                start = s;
                if (*s == '>') {
                    s++;
                    start = s;
                    state = 'A'; // redirect and append case
                }
                else {
                    state = 'R'; //redirect
                }
                break;
            case 's': // input case

                start = s;
                state = 'r';
                break;
            case 'r':
                if (*s == ' ') {
                    printstring(start, s, temp, 7,int_cmd, head, newVar, &first,file);
                    start = s;
                    state = ' ';
                }
                break;
            case 'R': //redirect
                if (*s == ' ') {
                    printstring(start, s, temp, 5,int_cmd, head, newVar, &first,file);
                    start = s;
                    state = ' ';
                }
                break;
            case 'A': //append and redirect
                if (*s == ' ') {

                    printstring(start, s, temp, 6,int_cmd, head, newVar, &first,file);
                    start = s;
                    state = ' ';
                }
                break;

            case 'Y':

                if (*s == '\\') {
                    printstring(start, s, temp, 3,int_cmd, head, newVar, &first,file);
                    start = s;
                    state = 'e';
                }
                else if (!((*s >= 'A' && *s <= 'Z') || (*s >= '0' && *s <= '9') || *s == '_') ){
                    printstring(start, s, temp, 3, int_cmd, head, newVar, &first,file);
                    start = s;
                    state = ' ';
                }
                break;

            case 'e':
                state = 'T';
                break;

            case 'E':
                state = '\"';
                break;

            case '\"': // Inside a quote
                if (*s == '$'){
                    printstring(start, s, temp, 1, int_cmd ,head, newVar, &first,file);
                    start = s;
                    state = 'V';
                }
                else if(*s == '\\'){
                    state = 'E';
                }
                else if (*s == '\"') {
                    printstring(start, s, temp, 1, int_cmd ,head, newVar, &first,file);
                    start = s;
                    state = ' '; // end quote
                }
                break;

        }
        s++;
    } // end while
    if (state == 'R') { //if text reaches end of buffer
        printstring(start, s, temp, 5, int_cmd, head, newVar, &first,file);
    }
    else if(state == 'r'){
        printstring(start, s, temp, 7, int_cmd, head, newVar, &first,file);
    }
    else if(state == 'A'){
        printstring(start, s, temp, 6, int_cmd, head, newVar, &first,file);
    }
    else if (state == 'Y') {
        printstring(start, s, temp, 3, int_cmd, head, newVar, &first,file);
    }
    else if (state != ' ') {
        printstring(start, s, temp, 0, int_cmd , head, newVar, &first,file);
    }
}

char *sgets(char *str, int size)
{
    char *ret;
    ret = fgets(str, size, stdin);
    if(!ret)
        return ret;
    if(str[strlen(str)-1] == '\n')
    {
        str[strlen(str)-1] = '\0';
    } else {
        while(getchar() != '\n');
    }
    return ret;
}

char *get_txt(char *str){

    FILE *fPointer;
    int c, v=0;
    fPointer = fopen("store.txt", "r");

    if (fPointer) {
        while ((c = getc(fPointer)) != EOF) {
            str[v] = (char) c;
            v++;
        }
    }

    return str;
}

void variables(var **head, var *newVar, char *argv, char *prompt){
        
        char allvar[PATH_AMOUNT][10]= {
                              "CWD",
                              "HOME",
                              "PATH",
                              "USER",
                              "TERMINAL",
                              "SHELL",
                              "PROMPT"
                            };

       char *homedir = getenv("HOME"); //home
       chdir(homedir);

       char cwd[PATH_MAX]; //cwd
       getcwd(cwd, sizeof(cwd));

       char *path = getenv("PATH"); //path

       char user[PATH_MAX]; //user
       getlogin_r(user, sizeof(user));

       char *term = ttyname((STDIN_FILENO)); //terminal

       char *shell = dirname(&(argv[0])); //shell

        char *allvarans[PATH_AMOUNT]= {
                              cwd,
                              homedir,
                              path,
                              user,
                              term,
                              shell,
                              prompt
                            };

       for(int i = 0; i< PATH_AMOUNT; i++){

        newVar = (struct variable_key_value*)malloc(sizeof(struct variable_key_value));

        strcpy(newVar->key, allvar[i]);
        strcpy(newVar->value, allvarans[i]);

        newVar->next = NULL;
        *head = add_node(*head, newVar);

       }

}