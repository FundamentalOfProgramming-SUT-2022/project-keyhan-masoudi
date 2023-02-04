#include <conio.h>
#include <sys/types.h>
#include <fileapi.h>
#include <sys/stat.h>
#include <windows.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////
#define maxsize 1000000
#define clip_add "\\root\\dir1\\clipboard.txt"
#define undo_save_add "\\cmake-build-debug\\undo_savefile.txt"
#define nist "File doesnt exist"
#define hast "File exist"
////////////////
////////////////
unsigned long long int length_of_string;
char path[100] , *func_name, dirname[50], space , attribute ;
char *script , *buffer , *buffer1 , *clipboard ,*check_name;
int pos_line , pos_char , num , counter_of_enter , invalid_check = 0 , flag_exist = 0;
////////////////
int file_exist(char address[]){
    if(access((address+1),F_OK) == 0){
        return 1;
    } else{
        return 0;
    }
}
////////////////
char *readfile(char address[]){
    if(!(file_exist(address))){
        printf("%s",nist);
        flag_exist = 1;
        return NULL;
    } else{
        buffer = (char*) calloc(maxsize, sizeof (char));
        char c;
        FILE *ptr;
        ptr = NULL;
        ptr = fopen(address + 1, "r");
        for (int m = 0; (c=(fgetc(ptr))) != EOF; m++) {
            buffer[m] = c;
        }
        fclose(ptr);
        return buffer;
    }
}
////////////////
char *hidden_read(char address[]){
    buffer = (char*) calloc(maxsize, sizeof (char));
    char c;
    FILE *ptr;
    ptr = NULL;
    SetFileAttributesA(clip_add+1,32);
    ptr = fopen(clip_add+1, "r");
    for (int m = 0; (c=(fgetc(ptr))) != EOF; m++) {
        buffer[m] = c;
    }
    fclose(ptr);
    SetFileAttributesA(clip_add+1,32+FILE_ATTRIBUTE_HIDDEN);
    return buffer;
}
////////////////
void writefile(char address[], char* matn){
    FILE *ptr;
    ptr = NULL;
    ptr = fopen(address + 1, "w");
    for (int i = 0;matn[i] != '\0'; ++i) {
        if (matn[i] == '\\' && matn[i - 1] != '\\') {
            if (matn[i + 1] == 'n') {
                fprintf(ptr, "%c", '\n');
                i++;
                continue;
            }
            else if (matn[i+1] == 't') {
                fprintf(ptr, "%c", '\t');
                i++;
                continue;
            }
            else if(matn[i+1] == '\\') {
                continue;
            }
            else{
                fprintf(ptr,"%c",matn[i]);
                continue;
            }
        }
        else if(matn[i] =='\\' && matn[i-1] == '\\'){
            fprintf(ptr,"%c",matn[i]);
            continue;
        }
        else {
            fprintf(ptr, "%c", matn[i]);
            continue;
        }
    }
    fclose(ptr);
}
////////////////
void writefile1(char address[], char* matn) {
    FILE *ptr;
    ptr = NULL;
    ptr = fopen(address + 1, "w");
    for (int i = 0;matn[i] != '\0'; ++i) {
        fprintf(ptr,"%c",matn[i]);
    }
    fclose(ptr);
}
////////////////
void hidden_write(char address[],char *matn){
    SetFileAttributesA(clip_add+1,32);
    writefile1(clip_add,matn);
    SetFileAttributesA(clip_add+1,32+FILE_ATTRIBUTE_HIDDEN);
}
///////////////
char *inputstr(void){
    char * inputs , c , b_slash;
    inputs = (char*) calloc(maxsize, sizeof(char));
    int i = 0;
        c = getchar();
        if(c == '"'){
            c = 'z';
            while (1){
                c = getchar();
                if((c == '"') && (b_slash == '\\')){
                    inputs[i] = c;
                    i++;
                    b_slash = 'k';
                } else if(c == '\\') {
                    b_slash = c;
                } else if((c == '"') && (b_slash != '\\')){
                    break;
                }
                else{
                    inputs[i] = c;
                    i++;
                }
            }
        } else{
            inputs[i] = c;
            i++;
            while (1){
                c = getchar();
                if(c == ' ' || c == '\n'){
                    break;
                } else {
                    inputs[i] = c;
                    i++;
                }
            }
        }
    printf("%s",inputs);
    return inputs;
}
///////////////
void undo_save (char *matn){
    FILE *pointer;
    pointer = NULL;
    pointer = fopen(undo_save_add+1,"w");
    writefile1(undo_save_add,matn);
    fclose(pointer);
}
///////////////
void last_undo(char address[]){
    char * undomatn;
    undomatn = (char*) calloc(maxsize,sizeof(char ));
    undomatn = readfile(undo_save_add);
    writefile1(address,undomatn);
}
///////////////
void create_file (char address[]) {
    address[strlen(address) + 1] = '\0';
    int counter = 0;
    for (int i = 0; address[i] != '\0'; ++i) {
        if(address[i] == '\\')
            counter++;
    }
    int counter1 = 0;
    for (int i = 0; address[i] != '\0'; ++i) {
        if(address[i] == '\\'){
            counter1++;
            if(counter == counter1)
                break;
            int j = i;
            dirname[j] = address[j];
            j++;
            for (; address[j] != '\\'; ++j) {
                dirname[j] = address[j];
                dirname[j+1] = '\0';
            }
            int x = mkdir(dirname+1);
            if(x == 0)
                i = j-1;
        }
    }
    if(file_exist(address)){
        printf("%s",hast);
        return;
    } else {
        FILE *newfile;
        newfile = fopen(address + 1, "w");
        fclose(newfile);
    }
}
////////////////
int enter(char matn[]){
    int counter = 0;
    for (int i = 0; matn[i] != '\0'; ++i) {
        if(matn[i] == '\n')
            counter++;
    }
    return counter;
}
////////////////
void insert(char address[]){
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    space = getchar();
    if(space != ' '){
        invalid_check = 1;
        return;
    }
    scanf("%s", check_name);
    if((strcmp(check_name,"--pos"))) {
        invalid_check = 1;
        return;
    }
    space = getchar();
    if(space != ' '){
        invalid_check = 1;
        return;
    }
    scanf("%d:%d",&pos_line,&pos_char);
    space = getchar();
    if(space != ' '){
        invalid_check = 1;
        return;
    }
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s", check_name);
    if(strcmp(check_name,"--str")) {
        invalid_check = 1;
        return;
    }
    space = getchar();
    if(space != ' '){
        invalid_check = 1;
        return;
    }
    /////////////////////
    script = (char*) calloc(maxsize , sizeof(char));
    buffer = (char*) calloc(maxsize, sizeof (char));
    buffer1 = (char*) calloc(maxsize,sizeof (char));
    buffer = readfile(address);
    undo_save(buffer);
    if(buffer == NULL){
        return;
    }
    counter_of_enter = enter(buffer);
    if(pos_line > counter_of_enter+1){
        invalid_check = 1;
        return;
    }
    char c;
    int i = 0 , j = 0;
    while (i < pos_line-1) {
        i++;
        while (buffer[j] != '\n'){
            script[j] = buffer[j];
            j++;
        }
        script[j] = '\n';
        j++;
    }
    for (int k = 0; k < pos_char; ++k) {
        script[j+k] = ' ';
    }
    int counter = 0;
    int j1 = 0;
    while(i < pos_line){
        i++;
        while ((buffer[j+counter] != '\0') && (buffer[j+counter] != '\n')) {
            if (counter < pos_char) {
            script[j + counter] = buffer[j + counter];
                j1 = j + counter;
            }
            counter++;
        }
    }
    j = j1;
    long int countofrest = 0 ;
    while (buffer[j1] != '\0'){
        j1++;
        countofrest++;
    }
    strncpy(buffer1,buffer+j+1,countofrest);
    buffer = (char*) calloc(maxsize,sizeof (char));
    scanf("%[^\n]s",buffer);
    strcat(script,buffer);
    strcat(script,buffer1);
    writefile(address, script);
}
////////////////
int check(char funcName[]){
    length_of_string = strlen(funcName);
    for (int i = 0; i < length_of_string; ++i) {
        if(funcName[i] == '-'){
            invalid_check = 1;
            return 0;
        }
    }
    if(!(strcmp(funcName, "createfile"))){
        return 1;
    }
    else if(!(strcmp(funcName, "insertstr"))){
        return 2;
    }
    else if(!(strcmp(funcName, "cat"))){
        return 3;
    }
    else if(!(strcmp(funcName, "removestr"))){
        return 4;
    }
    else if(!(strcmp(funcName, "copystr"))){
        return 5;
    }
    else if(!(strcmp(funcName, "cutstr"))){
        return 6;
    }
    else if(!(strcmp(funcName, "pastestr"))){
        return 7;
    }
    else if(!(strcmp(funcName, "undo"))){
        return 8;
    }
    else
        return -1;
}
////////////////
void cat(char address[]){
    script = (char*) calloc(maxsize , sizeof (char));
    script = readfile(address);
    if(script == NULL){
        return;
    } else{
    for (int i = 0;script[i] != '\0'; ++i) {
        printf("%c", script[i]);
    }
    }
}
////////////////
int  curser (int line , int p_char,char *matn){
    int counter = 1 , i = 0 ;
    while (counter < line ){
        if (matn[i] == '\n') {
            counter++;
        }
        i++;
    }
    i = i + p_char;
    return i;
}
////////////////
void removes(char address[]) {
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s", check_name);
    if (strcmp(check_name, "--pos") != 0) {
        invalid_check = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s%c", check_name, &space);
    if ((strcmp(check_name, "-size") != 0) || (space != ' ')) {
        invalid_check = 1;
        return;
    }
    char c;
    scanf("%d%c%c%c", &num, &space, &c, &attribute);
    if ((space != ' ') || (c != '-')) {
        invalid_check = 1;
        return;
    }
    int i = 0, j = 0;
    script = (char *) calloc(maxsize, sizeof(char));
    buffer = (char *) calloc(maxsize, sizeof(char));
    buffer = readfile(address);
    undo_save(buffer);
    if(buffer == NULL){
        return;
    }
    counter_of_enter = enter(buffer);
    length_of_string = strlen(buffer);
    int counter = 1;
    /////////////////////
    if (attribute == 'b'){
            while (counter < pos_line){
                if (buffer[i] == '\n') {
                    counter++;
                }
                i++;
            }
            for (; j < i + pos_char ; ++j) {
                script[j] = buffer[j];
            }
        for (;buffer[j] != '\0'; ++j) {
                script[j - num] = buffer[j];
            }
    }
        ////////////////////
        else if (attribute == 'f') {
            while (counter < pos_line){
                if (buffer[i] == '\n') {
                    counter++;
                }
                i++;
            }
            for (; j < i + pos_char ; ++j) {
                script[j] = buffer[j];
            }
            for (;buffer[j + num] != '\0'; ++j) {
                script[j] = buffer[j + num];
            }
        }
    writefile1(address,script);
        ///////////////////
}
////////////////
void copy(char address[]) {
    buffer = (char *) calloc(maxsize, sizeof(char));
    buffer = readfile(address);
    if(buffer == NULL){
        return;
    }
    space = getchar();
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s", check_name);
    if ((strcmp(check_name, "--pos") != 0) || (space != ' ')) {
        invalid_check = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s%c", check_name, &space);
    if ((strcmp(check_name, "-size") != 0) || (space != ' ')) {
         invalid_check = 1;
        return;
    }
    char c;
    scanf("%d%c%c%c", &num, &space, &c, &attribute);
    if ((space != ' ') || (c != '-')) {
        invalid_check = 1;
        return;
    }
    clipboard = (char *) calloc(maxsize, sizeof(char));
    int i, j = 0;
    i = curser(pos_line, pos_char, buffer);
    if (attribute == 'f') {
        for (int k = 0; k < num; ++k) {
            clipboard[k] = buffer[i + k];
        }
    } else if (attribute == 'b') {
        if(i < num){
            i = num;
        }
        for (int k = 0; k < num; ++k) {
            clipboard[k] = buffer[i - num + k];
        }
    } else {
        invalid_check = 1;
        return;
    }
    hidden_write(clip_add,clipboard);
}
////////////////
void cut (char address[]){
    buffer = (char *) calloc(maxsize, sizeof(char));
    script = (char *) calloc(maxsize,sizeof(char));
    buffer = readfile(address);
    if(buffer == NULL){
        return;
    }
    undo_save(buffer);
    space = getchar();
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s", check_name);
    if ((strcmp(check_name, "--pos") != 0) || (space != ' ')) {
        invalid_check = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s%c", check_name, &space);
    if ((strcmp(check_name, "-size") != 0) || (space != ' ')) {
        invalid_check = 1;
        return;
    }
    char c;
    scanf("%d%c%c%c", &num, &space, &c, &attribute);
    if ((space != ' ') || (c != '-')) {
        invalid_check = 1;
        return;
    }
    clipboard = (char *) calloc(maxsize, sizeof(char));
    int i, j = 0;
    i = curser(pos_line, pos_char, buffer);
    if (attribute == 'f') {
        for (int k = 0; k < i; ++k) {
            script[k] = buffer[k];
        }
        strcat(script,buffer+num+i);
        for (int k = 0; k < num; ++k) {
            clipboard[k] = buffer[i + k];
        }
    } else if (attribute == 'b') {
        if(i < num){
            i = num;
        }
        for (int k = 0; k < num; ++k) {
            clipboard[k] = buffer[i - num + k];
        }
    } else {
        invalid_check = 1;
        return;
    }
    writefile1(address,script);
    hidden_write(clip_add,clipboard);
}
////////////////
void paste(char address[]){
    clipboard = (char *) calloc(maxsize, sizeof(char));
    script = (char *) calloc(maxsize,sizeof(char));
    buffer1 = (char *) calloc(maxsize,sizeof(char));
    clipboard = readfile(address);
    undo_save(clipboard);
    space = getchar();
    free(check_name);
    check_name = (char*) calloc(20 , sizeof(char));
    scanf("%s", check_name);
    if ((strcmp(check_name, "--pos") != 0) || (space != ' ')) {
        invalid_check = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_check = 1;
        return;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    int i;
    i = curser(pos_line,pos_char,clipboard);
    strncat(script,clipboard,i);
    buffer1 = hidden_read(clip_add);
    strcat(script,buffer1);
    strcat(script,clipboard+i+1);
    writefile1(address,script);
}
////////////////

////////////////
int main(){
    int checkname;
    int flag = 1;
    invalid_check = 0;
    while(flag){
            if (invalid_check /*&&!(flag_exist)*/) {
                printf("INVALID COMMAND");
                invalid_check = 0;
                continue;
            }
        func_name = (char*) calloc(maxsize, sizeof(char));
        scanf("%s",func_name);
        space = getchar();
        if(space != ' '){
            invalid_check = 1;
        }
        free(check_name);
        check_name = (char*) calloc(20 , sizeof(char));
        scanf("%s", check_name);
        space = getchar();
        if(strcmp(check_name,"--file") != 0){
            invalid_check = 1;
        }
        if(space != ' '){
            invalid_check = 1;
        }
        checkname = check(func_name);
        scanf("%s",path);
        if(checkname == 1) {
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                create_file(path);
            }
        }
        else if(checkname == 2){
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                insert(path);
            }
        }
        else if(checkname == 3){
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                cat(path);
            }
        }
        else if(checkname == 4){
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                removes(path);
            }
        }else if(checkname == 5){
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                copy(path);
            }
        }else if(checkname == 6){
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                cut(path);
            }
        }else if(checkname == 7) {
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                paste(path);
            }
        }else if(checkname == 8) {
            if (invalid_check == 1) {
                printf("INVALID COMMAND");
                invalid_check = 0;
            } else {
                last_undo(path);
            }
        }
//        }else if(checkname == 8){
//            find(path);
//            if (// invalid_check = 1;) {
//                printf("INVALID COMMAND");
//                invalid_check = 0;
//            }
//        }
        //flag  = 0;
    }
}







/*\root\dir1\file.txt
createfile --file \root\dir1\file.txt*/