#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 


#define REPORT_FILE "packages_report.txt"

struct packageData {
    char *pkgName;
    char *pkgType;
    char *installDate;
    char *lastUpdate;
    char *removalDate;
    int howManyUpdates;
    int key;
};

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o \n");
	return 1;
    }

    analizeLog(argv[2], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char date[10];
    char *pkgName;
    char *pkgType;
    char * str3;

    bool inPkgType=false;
    


    fp = fopen(logFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        
        //CONSEGUIR LA FECHA
        for (size_t i=1;i<11;i++){
            char *tmp=(char *) malloc(strlen(str3));
            strcpy(tmp, str3);
            str3 = (char *) malloc(1 + strlen(str3));
            strcpy(str3, tmp);
            str3[i]=line[i];
        }

        strcpy(date, str3);
        printf("%s\n", date);

        //CONSEGUIR EL TIPO
       /* for (int i=11;i<strlen(line);i++){
            
            if (inPkgType){
                if (line[i+4] =='-'){
                    pkgType="ALPM-SCRIPLET";
                    //SUMAR AQUÍ
                }else if (line[i] =='A'){
                    pkgType="ALPM";
                    //SUMAR AQUÍ
                }else{
                    pkgType="PACMAN";
                    //SUMAR AQUÍ
                }
            }

            if (line[i] == '['){
                inPkgType=true;
            }

            
            
            printf("%c", line[i]);
        }
        */
        
        
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);


    printf("Report is generated at: [%s]\n", report);
}


