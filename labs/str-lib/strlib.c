#include <stdio.h>   
#include <stdlib.h>

int mystrlen(char *str){
    int tmpCounter = 0;
    while (*str != '\0') {
        tmpCounter++, str++;
    }
    return tmpCounter;
}

char *mystradd(char *origin, char *addition){
    printf("%s\t%d\n","Initial Length: ",mystrlen(origin));
    int len1= mystrlen(origin);
    int len2= mystrlen(addition);
    char newStr[len1 + len2];
    int i=0;
    while (*origin!= '\0') {
        newStr[i]=*origin;
        origin++,i++;
    }
    while(*addition != '\0'){
        newStr[i]=*addition;
        addition++,i++;
    }
    newStr[i]='\0';
    printf("New String: %s\n", newStr); 
    printf("New length: %i\n", mystrlen(newStr));
    return 0;
}

int mystrfind(char *origin, char *substr){
    printf("%s%s%s","['",substr,"']"); 
    int pos = 0;
    while (*origin!= '\0') {
        while(*origin == *substr){
            origin++, substr++;
            if(*substr == '\0'){
               printf("%s%s%d%s%s\n"," string was found at ","[",pos,"]"," position");  
               return 0;
            }
        }
        substr=&substr[0], pos++, origin++;
    }
    printf("%s\n","Non existant "); 
    return 0;
}