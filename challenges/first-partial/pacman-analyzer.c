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
    char *operationType;
    char *pkgType;
    
    char * str3;
    
    int numberALPM=0
        , numberPACMAN=0
        , numberALPMS=0;

    
    


    fp = fopen(logFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        

        int currentChar=1;

        bool inPkgType=false;
        bool isCommonString=false;

        
        
        
        //CONSEGUIR LA FECHA
        for (currentChar;currentChar<11;currentChar++){
            char *tmp=(char *) malloc(strlen(str3));
            strcpy(tmp, str3);
            str3 = (char *) malloc(1 + strlen(str3));
            strcpy(str3, tmp);
            str3[currentChar]=line[currentChar];
        }

        strcpy(date, str3);
        printf("%s\n", date);

        //CONSEGUIR EL TIPO
        for (currentChar;currentChar<read;currentChar++){
            
            if (inPkgType){
                if (line[currentChar+4] =='-'){
                    pkgType="ALPM-SCRIPLET";
                    numberALPMS++;
                    currentChar+=13;
                    inPkgType=false;
                    break;
                }else if (line[currentChar] =='A'){
                    pkgType="ALPM";
                    numberALPM++;
                    currentChar+=4;
                    inPkgType=false;
                    break;
                }else{
                    pkgType="PACMAN";
                    numberPACMAN++;
                    currentChar+=6;
                    inPkgType=false;
                    break;
                }
            }

            if (line[currentChar] == '['){
                inPkgType=true;
            }
        }

        printf("%s\n", pkgType);
        

        
        //CONSEGUIR INSTALLED (REINSTALLED), UPGRADED OR REMOVED
           if (line[currentChar] =='i'  && line[currentChar+1] =='n' && line[currentChar+2] =='s' && line[currentChar+3] =='t'){
                strcpy(operationType,"install");
                
            }else if (line[currentChar] =='r'  && line[currentChar+1] =='e' && line[currentChar+2] =='i' && line[currentChar+3] =='n'){
                strcpy(operationType,"install");
                
            }else if (line[currentChar] =='r'  && line[currentChar+1] =='e' && line[currentChar+2] =='m' && line[currentChar+3] =='o'){
                strcpy(operationType,"removed");
                
            }else if (line[currentChar] =='u'  && line[currentChar+1] =='p' && line[currentChar+2] =='g' && line[currentChar+3] =='r'){
                strcpy(operationType,"upgraded");
                
            }else{
                isCommonString=true;
            }
        
        
    }

    printf("Number of ALPM: %i\n Number of ALPM-SCRIPLET: %i\n Number of PACMAN: %i\n", numberALPM, numberALPMS, numberPACMAN);
        
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);


    printf("Report is generated at: [%s]\n", report);
}


