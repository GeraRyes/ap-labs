#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 


#define REPORT_FILE "packages_report.txt"

struct packageData {
    char *pkgName;
    char *scriptType;
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

    
    int numberALPM=0, numberPACMAN=0, numberALPMS=0;
    char *pkgName, *scriptType, *date, *operationType, *isInstalled, *wasUpgraded, *howManyUpgrades; 

    fp = fopen(logFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        
        char *storedChar;
        bool desechar=false;
        int from, to, i, j;
        
        
        if (line[0]=='['){

            //TOMA EL VALOR DE LA FECHA
            storedChar = strtok(line, "]");
            from=1;
            to=10;
            date=(char*)malloc ( 11*sizeof(char) );

            for(i=from,j=0; i<=to; i++,j++){
                
		        date[j]=storedChar[i];
                
            }
            printf("%s\n", date);
        
            

            //TOMA EL VALOR DEL TIPO DE SCRIPT 
            storedChar = strtok(NULL, " ");
            scriptType=(char*)malloc ( 11*sizeof(char) );

            //CONTAR EL SCRIPT PARA LOS VALORES FINALES
            if (storedChar=="[PACMAN]"){
                numberPACMAN++;
            }else if(storedChar=="[ALPM]"){
                numberALPM++;
            }else{
                numberALPMS++;
            }

            //TIPO DE OPERACION(INSTALLED, REINSTALLED, REMOVED, UPGRADED)
            storedChar = strtok(NULL, " ");
            printf("%s ", storedChar);

            if (storedChar=="installed" || storedChar=="reinstalled" || storedChar=="removed" || storedChar=="upgraded"){

                //NOMBRE DEL PKG
                storedChar = strtok(NULL, " ");
                printf("%s\n", storedChar);
                
            }else{
                desechar=true;
            }

            

        }
        
        
        /*

        int currentChar=1;

        bool inPkgType=false;
        bool isCommonString=false;

        
        
        
        //CONSEGUIR LA FECHA
        for (currentChar;currentChar<11;currentChar++){
            char *tmp=(char *) malloc(strlen(strDate));
            strcpy(tmp, strDate);
            strDate = (char *) malloc(1 + strlen(strDate));
            strcpy(strDate, tmp);
            strDate[currentChar]=line[currentChar];
            free(tmp);
        }

        strcpy(date, strDate);
        free(strDate);
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
            strcpy(operationType,"installed");
            currentChar+=9;
                
        }else if (line[currentChar] =='r'  && line[currentChar+1] =='e' && line[currentChar+2] =='i' && line[currentChar+3] =='n'){
            strcpy(operationType,"installed");
            currentChar+=11;
            
        }else if (line[currentChar] =='r'  && line[currentChar+1] =='e' && line[currentChar+2] =='m' && line[currentChar+3] =='o'){
            strcpy(operationType,"removed");
            currentChar+=7;
        }else if (line[currentChar] =='u'  && line[currentChar+1] =='p' && line[currentChar+2] =='g' && line[currentChar+3] =='r'){
            strcpy(operationType,"upgraded");
            currentChar+=8;
        }else{
            isCommonString=true;
        }

        //CONSEGUIR PKGNAME
        int counter=0;
        int tmpCounter=1;
        char *tmpCounter
       
        while (line[currentChar]!=' '){
            //char *tmp =  malloc(tmpCounter * sizeof(char*));
            /*strcpy(tmp2, strPkgName);
            strPkgName = (char *) malloc(1 + strlen(strPkgName));
            strcpy(strPkgName, tmp2);
            strDate[counter]=line[currentChar];
            counter++;
            currentChar++;
            */
        }
            //strcpy(pkgName, strPkgName);
        
        /*if (operationType="install"){
            
        }
    }

    printf("Number of ALPM: %i\n Number of ALPM-SCRIPLET: %i\n Number of PACMAN: %i\n", numberALPM, numberALPMS, numberPACMAN);
        
    */

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);


    printf("Report is generated at: [%s]\n", report);
}


