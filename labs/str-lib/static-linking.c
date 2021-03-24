#include <stdio.h>
#include <string.h> 

int mystrfind(char*,char*);
int mystrlen(char*);
char *mystradd(char*,char*);


int main(int argc, char **argv) {
    if(argc >= 4){
        char *arg1= argv[2];
        char *arg2 = argv[3];
        char *library=argv[1];
        if(strcmp(library,"-add") == 0){
            mystradd(arg1, arg2);
        }else{
            mystrfind(arg1, arg2);
        }
    }else{
        printf("Error\n");
        
    }
    return 0;
}
