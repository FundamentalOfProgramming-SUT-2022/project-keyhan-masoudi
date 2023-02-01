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
#define maxsize 5000
#define clip_add "\\root\\dir1\\clipboard.txt"
////////////////
////////////////
unsigned long long int length_of_string;
char path[100] , *func_name, check_name[20] , dirname[50], space , attribute ;
char *script , *buffer , *buffer1 , *clipboard;
int pos_line , pos_char , num , counter_of_enter , invalid_ckeck = 0;
////////////////
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
    FILE *newfile;
    newfile = fopen(address + 1, "w");
    fclose(newfile);
}
////////////////
char *readfile(char address[]){
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
////////////////
char * hidden_read(char address[]){
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
void hidden_write(char address[],char *matn){
    SetFileAttributesA(clip_add+1,32);
    writefile(clip_add,matn);
    SetFileAttributesA(clip_add+1,32+FILE_ATTRIBUTE_HIDDEN);
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
    space = getchar();
    if(space != ' '){
        invalid_ckeck = 1;
        return;
    }
    scanf("%s", check_name);
    if(strcmp(check_name,"--pos")) {
        invalid_ckeck = 1;
        return;
    }
    space = getchar();
    if(space != ' '){
        invalid_ckeck = 1;
        return;
    }
    scanf("%d:%d",&pos_line,&pos_char);
    space = getchar();
    if(space != ' '){
        invalid_ckeck = 1;
        return;
    }
    scanf("%s", check_name);
    if(strcmp(check_name,"--str")) {
        invalid_ckeck = 1;
        return;
    }
    space = getchar();
    if(space != ' '){
        invalid_ckeck = 1;
        return;
    }
    /////////////////////
    /////////////////////
    /////////////////////
    script = (char*) calloc(maxsize , sizeof(char));
    buffer = (char*) calloc(maxsize, sizeof (char));
    buffer1 = (char*) calloc(maxsize,sizeof (char));
    buffer = readfile(address);
    counter_of_enter = enter(buffer);
    if(pos_line > counter_of_enter+1){
        invalid_ckeck = 1;
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
            invalid_ckeck = 1;
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
    else
        return -1;
}
////////////////
void cat(char address[]){
    script = (char*) calloc(maxsize , sizeof (char));
    script = readfile(address);
    for (int i = 0;script[i] != '\0'; ++i) {
        printf("%c",script[i]);
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
    buffer = (char *) calloc(maxsize, sizeof(char));
    buffer = readfile(address);
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
        return;
    }
    scanf("%s", check_name);
    if (strcmp(check_name, "--pos") != 0) {
        invalid_ckeck = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
        return;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
        return;
    }
    scanf("%s%c", check_name, &space);
    if ((strcmp(check_name, "-size") != 0) || (space != ' ')) {
        invalid_ckeck = 1;
        return;
    }
    char c;
    scanf("%d%c%c%c", &num, &space, &c, &attribute);
    if ((space != ' ') || (c != '-')) {
        invalid_ckeck = 1;
        return;
    }
    int i = 0, j = 0;
    script = (char *) calloc(maxsize, sizeof(char));
    buffer = (char *) calloc(maxsize, sizeof(char));
    buffer = readfile(address);
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
    writefile(address,script);
        ///////////////////
}
////////////////
void copy(char address[]) {
    buffer = (char *) calloc(maxsize, sizeof(char));
    buffer = readfile(address);
    space = getchar();
    scanf("%s", check_name);
    if ((strcmp(check_name, "--pos") != 0) || (space != ' ')) {
        invalid_ckeck = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
        return;
    }
    scanf("%s%c", check_name, &space);
    if ((strcmp(check_name, "-size") != 0) || (space != ' ')) {
        invalid_ckeck = 1;
        return;
    }
    char c;
    scanf("%d%c%c%c", &num, &space, &c, &attribute);
    if ((space != ' ') || (c != '-')) {
        invalid_ckeck = 1;
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
        for (int k = 0; k < num; ++k) {
            clipboard[k] = buffer[i - num + k - 1];
        }
    } else {
        invalid_ckeck = 1;
        return;
    }
    hidden_write(clip_add,clipboard);
}
////////////////
void cut (char address[]){
    buffer = (char *) calloc(maxsize, sizeof(char));
    script = (char *) calloc(maxsize,sizeof(char));
    buffer = readfile(address);
    space = getchar();
    scanf("%s", check_name);
    if ((strcmp(check_name, "--pos") != 0) || (space != ' ')) {
        invalid_ckeck = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
        return;
    }
    scanf("%s%c", check_name, &space);
    if ((strcmp(check_name, "-size") != 0) || (space != ' ')) {
        invalid_ckeck = 1;
        return;
    }
    char c;
    scanf("%d%c%c%c", &num, &space, &c, &attribute);
    if ((space != ' ') || (c != '-')) {
        invalid_ckeck = 1;
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
        for (int k = 0; k < num; ++k) {
            clipboard[k] = buffer[i - num + k - 1];
        }
    } else {
        invalid_ckeck = 1;
        return;
    }
    writefile(address,script);
    hidden_write(clip_add,clipboard);
}
////////////////
void paste(char address[]){
    clipboard = (char *) calloc(maxsize, sizeof(char));
    script = (char *) calloc(maxsize,sizeof(char));
    buffer1 = (char *) calloc(maxsize,sizeof(char));
    clipboard = readfile(address);
    space = getchar();
    scanf("%s", check_name);
    if ((strcmp(check_name, "--pos") != 0) || (space != ' ')) {
        invalid_ckeck = 1;
        return;
    }
    space = getchar();
    if (space != ' ') {
        invalid_ckeck = 1;
    }
    scanf("%d:%d", &pos_line, &pos_char);
    int i;
    i = curser(pos_line,pos_char,clipboard);
    strncat(script,clipboard,i);
    buffer1 = hidden_read(clip_add);
    strcat(script,buffer1);
    strcat(script,clipboard+i+1);
    writefile(address,script);
}
////////////////
int main(){
    int checkname;
    int flag = 1;
//    int i;
//    buffer = (char*) calloc(maxsize, sizeof (char));
//    buffer = readfile("\\root\\dir1\\file1.txt");
//    i = curser(1,7,buffer);
//    printf("%d",i);
    while(flag){
        func_name = (char*) calloc(maxsize, sizeof(char));
        scanf("%s",func_name);
        space = getchar();
        if(space != ' '){
            invalid_ckeck = 1;
            continue;
        }
        scanf("%s", check_name);
        space = getchar();
        if(strcmp(check_name,"--file") != 0){
            invalid_ckeck = 1;
            continue;
        }
        if(space != ' '){
            invalid_ckeck = 1;
            continue;
        }
        checkname = check(func_name);
        scanf("%s",path);
        if(checkname == 1) {
            create_file(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }
        else if(checkname == 2){
            insert(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }
        else if(checkname == 3){
            cat(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }
        else if(checkname == 4){
            removes(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }else if(checkname == 5){
            copy(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }else if(checkname == 6){
            cut(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }else if(checkname == 7){
            paste(path);
            if (invalid_ckeck) {
                printf("INVALID COMMAND");
                invalid_ckeck = 0;
            }
        }
    }
    return 0 ;
}
/*\root\dir1\file.txt
createfile --file \root\dir1\file.txt*/