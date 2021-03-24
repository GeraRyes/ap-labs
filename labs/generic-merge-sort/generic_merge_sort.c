#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

char *objects[1500];
int size = 0;
int isNumber = 0;

void getObjects(char *text);
int compareInt(char *s1,char *s2);
void mergeSort(void *objects[],int start,int end, int (*comparator)(void* , void*));
void merge(void *objects[],int start,int middle,int end, int (*comparator)(void *, void *));
void print();

int main(int argc,char **argv)
{
    if(argc < 2){
        return 0;
    }
    if(argc == 2 && strcmp(argv[1],"-n") == 0){
        return 0;
    }
    if(argc == 3 && strcmp(argv[1],"-n") == 0){
        isNumber = 1;
    }
    if(isNumber == 0){
        getObjects(argv[1]);
    }else{
        getObjects(argv[2]);
    }
    mergeSort((void *)objects,0, size - 1,(int (*)(void*, void*))(isNumber ? *compareInt : strcmp));
    print();
    return 0;
}

void mergeSort(void *objects[],int start,int end, int (*comparator)(void *, void *)){
    int middle;
    if(start < end){
        middle = start + (end - start)/2;
        mergeSort((void *)objects, start, middle,(int (*)(void*, void*))(isNumber ? compareInt : strcmp)); 
        mergeSort((void *)objects, middle + 1, end,(int (*)(void*, void*))(isNumber ? compareInt : strcmp)); 
        merge((void *)objects, start, middle, end,(int (*)(void*, void*))(isNumber ? compareInt : strcmp));
    }
}

void merge(void *objects[],int start,int middle,int end, int (*comparator)(void *, void *)){
    int tmp1 = middle - start + 1;
    int tmp2=  end - middle;
    void *first[tmp1];
    void *second[tmp2];
    for(int x = 0 ; x < tmp1 ; x++){
        first[x]=objects[start+x];
    }
    for(int x = 0 ; x < tmp2 ; x++){
        second[x]=objects[middle+1+x];
    }
    int x = 0;
    int y = 0;
    int z = start;
    while(x < tmp1 && y < tmp2){
        if((*comparator)(first[x], second[y]) < 0){
            objects[z] = first[x];
            x++;
        }else{
            objects[z] = second[y];
            y++;
        }

        z++;
    }

    while(x < tmp1){
            objects[z] = first[x];
            x++;
            z++;
    }

    while(y < tmp2){
            objects[z] = second[y];
            y++;
            z++;
    }
}

void getObjects(char *text){
    FILE *file;
    file = fopen(text, "r");
    int pos = 0;
    char characters[1500] = {0};
    if (file == NULL) {
        return;
    }
    while(fgets(characters, 100, file)){
        objects[pos] = (char*)malloc(strlen(characters) + sizeof(char*));
        strcpy(objects[pos],characters);
        pos++;
        size++;
    }
    if(fclose(file)){
    }
}

int compareInt(char *s1,char *s2){
    double d1,d2;
    d1 = atof(s1);
    d2 = atof(s2);
    if(d1 < d2){
        return -1;
    }else if(d1 > d2){
        return 1;
    }else{
        return 0;
    }
}

void print(){
    for (int x = 0 ; x < size ; x++){
        printf("%s", objects[x]);
    }
} 