#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

//CAPACIDAD DE LA HASHTABLE
#define CAPACITY 4000 
#define REPORT_FILE "packages_report.txt"


//ESTOS SON NUESTROS OBJETOS QUE CONTIENEN LA INFORMACION DE LOS PAQUETES
typedef struct packageData packageData;

struct packageData {
    char *pkgName;
    char *installDate;
    char *lastUpdate;
    char *removalDate;
    int howManyUpdates;
    bool isInstalled;
};

//ESTO DEFINE NUESTRA HASHTABLE
typedef struct HashTable HashTable;
 

struct HashTable {

    packageData** items;
    int size;
    int count;
};

typedef struct FileData FileData;
 

struct FileData {

    int ALPM;
    int ALPMS;
    int PACMAN;
    char *oldestPkg;
    char *newestPkg;
    char *noUpgrades[20];
    int upgradelessNo;
    int installed;
    int removed;
    int upgraded;

};

//SE CREA UNA NUEVA HASTABLE
HashTable* create_table(int size) {
   
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (packageData**) calloc (table->size, sizeof(packageData*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
 
    return table;
}
 

//SE CREA UN NUEVO ITEM DE TIPO DATA DE PAQUETE
packageData* createItem(char *pkgName, char *date, char* operationType) {
    
    
    packageData* item = (packageData*) malloc (sizeof(packageData));
    item->installDate = (char*) malloc (strlen(date) + 1);
    item->removalDate = (char*) malloc (strlen(date) + 1);
    item->lastUpdate = (char*) malloc (strlen(date) + 1);
    item->pkgName = (char*) malloc (strlen(pkgName) + 1);
    item->howManyUpdates=0;
    strcpy(item->pkgName, pkgName);

    if (strcmp(operationType, "i") == 0){
        
        strcpy(item->installDate, date);
    }else if (strcmp(operationType, "r") == 0){
        
        strcpy(item->removalDate, date);
    }else if (strcmp(operationType, "u") == 0){
        
        strcpy(item->lastUpdate, date);
    }
    return item;
}

//FUNCIÓN QUE SIRVE PARA HACER EL HASHING Y REGRESA LA POSICION
unsigned long hash_function(char* str) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++){
        i += str[j];
    }
    return i % CAPACITY;
}

 //INSERTA UN PKG A LA HASHTABLE    
void ht_insert(HashTable* table, packageData *item, char* operationType) {

    //SE LLAMA AL HASHING Y ESE SERÁ EL INDEX DEL STRUCT
    unsigned long index = hash_function(item->pkgName);
 
    packageData* currentItem = table->items[index];
     
     //SI EL ESPACIO ESTÁ VACIO
    if (currentItem == NULL) {
        
        if (table->count == table->size) {
            printf("Insert Error: Hash Table is full\n");

            return;
        }

        table->items[index] = item; 
        table->count++;
    }
 
    else {
            //SI REQUIERE SOLO ACTUALIZAR VALORES
            if (strcmp(currentItem->pkgName, item->pkgName) == 0) {

                if (strcmp(operationType, "r") == 0){
                    
                    strcpy(table->items[index]->removalDate, item->removalDate);
                    return;
                    
                }else if (strcmp(operationType, "i") == 0){
                    
                    strcpy(table->items[index]->installDate, item->installDate);
                    return;
                    
                }else if (strcmp(operationType, "u") == 0 ){
                    
                    strcpy(table->items[index]->lastUpdate, item->lastUpdate);
                    table->items[index]->howManyUpdates++;
                    return;
                    
                }{

                }
                
            }
     
        else {
            return;
        }
    }
}
 
 //BUSCA EL PAQUETE, SI NO LO ENCUENTRA, DA NULL
packageData* htSearch(HashTable* table, char* pkgName) {

    int index = hash_function(pkgName);
    packageData* item = table->items[index];
 
    // Ensure that we move to a non NULL item
    if (item != NULL) {
        if (strcmp(item->pkgName, pkgName) == 0)
            return item;
    }
    return NULL;
}

//ESCRIBE LA TABLA, ELEMENTO POR ELEMENTO
void writeTable(HashTable* table, FILE *fp) {
    for (int i=0; i<table->size; i++) {
        if (table->items[i]) {
            fputs("- Package Name          : ", fp);
            fputs(table->items[i]->pkgName, fp);
            fputs("\n", fp);
            fputs("   - Install date       : ", fp);
            fputs(table->items[i]->installDate, fp);
            fputs("\n", fp);
            fputs("   - Last update date   : ", fp);
            fputs(table->items[i]->lastUpdate, fp);
            fputs("\n", fp);
            fputs("   - How many updates   : ", fp);
            fprintf(fp, "%d", table->items[i]->howManyUpdates);
            fputs("\n", fp);
            fputs("   - Removal date       : ", fp);
            fputs(table->items[i]->removalDate, fp);
            fputs("\n", fp);
        }
    }
}

void writeUpgradeless(HashTable* table, FILE *fp) {

    for (int i=0; i<table->size; i++) {
        if (table->items[i] && table->items[i]!=NULL) {
            if (table->items[i]->howManyUpdates == 0)
            fputs(table->items[i]->pkgName, fp);
            fputs(" ", fp);

        }
    }
    fputs("\n", fp);
}

void analizeLog(char *logFile, char *report);
int cfileexists(const char * filename);
void writeOutput(char * report, HashTable *table, FileData *stats);


//MAIN
int main(int argc, char **argv) {

    //SI SE PASAN MENOS ARGUMENTOS DE LOS 4 QUE SON NECESARIOS
    if (argc < 4) {
	printf("Invalid number or agruments, should be running like this: \n");
    printf("./pacman-analyzer -input source.txt -output destination.txt\n");
	return 0;
    }

    if (cfileexists(argv[2]) == 1){
        analizeLog(argv[2], argv[4]);
        return 0;
    }else{
        fprintf(stderr, "Can't open input file!\n");
        return 0;
    }
    
}

//COMPROBAR QUE EXISTA EL ARCHIVO A LEER
int cfileexists(const char * filename){
    
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}

//AQUI SE ESCRIBE EL DOCUMENTO DE SALIDA
void writeOutput(char *report, HashTable *table, FileData *stats){

    FILE *fp = fopen (report, "w");
    fputs("Pacman Packages Report\n", fp);
    fputs("-------------------------\n", fp);  
    fputs("- Installed packages    : ", fp);
    fprintf(fp, "%d\n", stats->installed);
    fputs("- Removed packages      : ", fp);
    fprintf(fp, "%d\n", stats->removed);
    fputs("- Upgraded packages     : ", fp);
    fprintf(fp, "%d\n", stats->upgraded);
    fputs("- Currently installed   : ", fp);
    fprintf(fp, "%d\n", (stats->installed - stats->removed));
    fputs("-------------------------\n", fp);
    fputs("General Stats\n", fp);
    fputs("-------------------------\n", fp);
    
    //WITHOUT UPGRADES
    fputs("- Without upgrades    : ", fp);
    writeUpgradeless(table, fp);
    fputs("\n", fp);

    //SCRIPT TYPES
    fputs("- [ALPM-SCRIPLET] count  : ", fp);
    fprintf(fp, "%d\n", stats->ALPMS);
    fputs("- [ALPM] count           : ", fp);
    fprintf(fp, "%d\n", stats->ALPM);
    fputs("- [PACMAN] count         : ", fp);
    fprintf(fp, "%d\n", stats->PACMAN);
    
    //INDIVIDUAL PKGS
    fputs("-------------------------\n", fp);
    fputs("List of Packages\n", fp);
    fputs("-------------------------\n", fp);
    writeTable(table, fp);

    fclose(fp);
}

//AQUI VA LA MAYORIA DEL ANALISIS DEL DOCUMENTO DE INPUT
void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    FILE *fp;
    FILE *fpOutput;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int numberALPM=0, numberPACMAN=0, numberALPMS=0;
    char *pkgName, *date, *operationType;
    char *storedChar;
    char ins[]="i";
    char rem[]="r";
    char upg[]="u";
    int installed=0, removed=0, upgraded=0;

    
    HashTable* ht = create_table(CAPACITY);
    

    fp = fopen(logFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        
        
        int from, to, i, j;
        
        
        if (line[0]=='['){

            //TOMA EL VALOR DE LA FECHA
            storedChar = strtok(line, "]");

            from=1;
            to=10;
            date=(char*)malloc ( strlen(storedChar)+1 );
            strncpy(date, storedChar+1, 10);


            //TOMA EL VALOR DEL TIPO DE SCRIPT 
            storedChar = strtok(NULL, " ");

            //CONTAR EL SCRIPT PARA LOS VALORES FINALES
            if ((strcmp(storedChar, "[PACMAN]") == 0)){
                numberPACMAN++;
            }else if((strcmp(storedChar, "[ALPM]") == 0)){
                numberALPM++;
            }else{
                numberALPMS++;
            }

            //TIPO DE OPERACION(INSTALLED, REINSTALLED, REMOVED, UPGRADED)
            storedChar = strtok(NULL, " ");
            

            if (strcmp(storedChar, "installed") == 0 || strcmp(storedChar, "reinstalled") == 0 || strcmp(storedChar, "removed") == 0 || strcmp(storedChar, "upgraded") == 0){

                
                if (strcmp(storedChar, "installed") == 0 || strcmp(storedChar, "reinstalled") == 0){
                    operationType= (char*)malloc(2);
                    memcpy(operationType, ins, 1);
                    installed++;
                }else if (strcmp(storedChar, "removed") == 0){
                    operationType= (char*)malloc(2);
                    memcpy(operationType, rem, 1);
                    removed++;
                }else{
                    operationType= (char*)malloc(2);
                    memcpy(operationType, upg, 1);
                    upgraded++;
                }
                

                //NOMBRE DEL PKG
                storedChar = strtok(NULL, " ");
                pkgName=storedChar;

                

                packageData* item = createItem(pkgName, date, operationType);
                ht_insert(ht, item, operationType);
                free(operationType);
                
            }else{

            }
        free(date);
        }
        
    }

    FileData* stats = (FileData*) malloc (sizeof(FileData));
    stats->installed=installed;
    stats->removed=removed;
    stats->upgraded=upgraded;
    stats->ALPMS=numberALPMS;
    stats->ALPM=numberALPM;
    stats->PACMAN=numberPACMAN;
    writeOutput(report,ht, stats);

    printf("Report is generated at: [%s]\n", report);

    //print_table(ht);
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

    
    
}


