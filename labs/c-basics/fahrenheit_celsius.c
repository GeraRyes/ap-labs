#include <stdio.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
    if (argc==1){
        printf("Please add another parameter to perform calculations");
        return 1;
    }else if (argc==2){
        char *a = argv[1];
        int fahr = atoi(a);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    }else if(argc==4){
        int fahr;

        char *a = argv[1];
        int low = atoi(a);

        char *b = argv[2];
        int high = atoi(b);

        char *c = argv[3];
        int stp = atoi(c);


        for (fahr = low; fahr <= high; fahr = fahr + stp)
	    printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

        return 0;
    }else{
        printf("Error, numero incorrecto de parametros");
        return 1;
    }
    
}