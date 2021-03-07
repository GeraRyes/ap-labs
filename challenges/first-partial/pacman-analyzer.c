#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

//CAPACIDAD DE LA HASHTABLE
#define CAPACITY 400 
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
packageData* createItem(char *pkgName, char *date, int operationType) {
    
    
    packageData* item = (packageData*) malloc (sizeof(packageData));
    item->pkgName = (char*) malloc (strlen(pkgName) + 1);
    strcpy(item->pkgName, pkgName);

    if (operationType==0){
        item->installDate = (char*) malloc (strlen(date) + 1);
        
        strcpy(item->installDate, date);
    }else if (operationType==1){
        item->removalDate = (char*) malloc (strlen(date) + 1);
        strcpy(item->removalDate, date);
    }else if (operationType==2){
        item->lastUpdate = (char*) malloc (strlen(date) + 1);
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

void handle_collision(HashTable* table, unsigned long index, packageData* item) {
}
 
void ht_insert(HashTable* table, packageData *item, int operationType) {

    // Compute the index
    unsigned long index = hash_function(item->pkgName);
 
    packageData* currentItem = table->items[index];
     
    if (currentItem == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");

            return;
        }
         
        // Insert directly
        table->items[index] = item; 
        table->count++;
    }
 
    else {
            // Scenario 1: We only need to update value
            if (strcmp(currentItem->pkgName, item->pkgName) == 0) {

                if (strcmp(operationType, "removed") == 0){
                    
                    strcpy(table->items[index]->removalDate, item->removalDate);
                    table->items[index]->isInstalled=false;
                    return;
                    
                }else if (strcmp(operationType, "installed") == 0 || strcmp(operationType, "reinstalled") == 0){
                    
                    strcpy(table->items[index]->installDate, item->installDate);
                    table->items[index]->isInstalled=true;
                    return;
                    
                }else if (strcmp(operationType, "upgraded") == 0 ){
                    
                    strcpy(table->items[index]->lastUpdate, item->lastUpdate);
                    table->items[index]->howManyUpdates++;
                    table->items[index]->isInstalled=true;
                    return;
                    
                }{

                }
                
            }
     
        else {//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            // Scenario 2: Collision
            // We will handle case this a bit later
            handle_collision(table, index, item);
            return;
        }
    }
}
 
packageData* htSearch(HashTable* table, char* pkgName) {
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    int index = hash_function(pkgName);
    packageData* item = table->items[index];
 
    // Ensure that we move to a non NULL item
    if (item != NULL) {
        if (strcmp(item->pkgName, pkgName) == 0)
            return item;
    }
    return NULL;
}

void print_table(HashTable* table) {
    printf("\nHash Table\n-------------------\n");
    for (int i=0; i<table->size; i++) {
        if (table->items[i]) {
            printf("Index:%d, Nombre Paquete:%s\n", i, table->items[i]->pkgName);
        }
    }
    printf("-------------------\n\n");
}


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

    int numberALPM=0, numberPACMAN=0, numberALPMS=0, operationType;
    char *pkgName, *date, *isInstalled, *wasUpgraded, *howManyUpgrades;
    
    HashTable* ht = create_table(CAPACITY);
    

    fp = fopen(logFile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        
        char *storedChar;
        bool desechar=false;
        int from, to, i, j;
        bool isRemoved=false;
        
        
        if (line[0]=='['){

            //TOMA EL VALOR DE LA FECHA
            storedChar = strtok(line, "]");

            from=1;
            to=10;
            date=(char*)malloc ( 11*sizeof(char) );

            for(i=from,j=0; i<=to; i++,j++){
                
		        date[j]=storedChar[i];
                
            }

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
                    operationType=0;
                }else if (strcmp(storedChar, "removed") == 0){
                    operationType=1;
                }else if (strcmp(storedChar, "upgraded") == 0){
                    operationType=2;
                }else{
                    printf("error");
                }
                
                //NOMBRE DEL PKG
                storedChar = strtok(NULL, " ");
                pkgName=storedChar;

                printf("%s %s %i\n ", date, pkgName, operationType);

                packageData* item = createItem(pkgName, date, operationType);
                ht_insert(ht, item, operationType);
                
            }

            

        }
        
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

    print_table(ht);
    printf("Report is generated at: [%s]\n", report);
}


