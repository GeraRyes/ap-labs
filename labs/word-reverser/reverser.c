#include <stdio.h>

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */

void reverse(int lenght, char arr[]) {

    int i;
    char tmp;

    for (i = 0;  i < lenght/2; i++) {
	tmp = arr[i];
	arr[i] = arr[lenght - i - 1];
	arr[lenght - i - 1] = tmp;
    }
}

int main()

{
    int pos=0;
    char c, word[100];
    while((c=getchar()) !=EOF){
        if (c=='\n'){
            reverse(pos, word);
            for(int i=0;i<pos;i++){
                printf("%c", word[i]);
            }
            pos=0;
        }else{
            word[pos]=c;
            pos++;
        }
    }
    return 0;
}