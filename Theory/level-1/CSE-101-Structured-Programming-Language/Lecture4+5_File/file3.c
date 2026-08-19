#include<stdio.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    int i = 0;
    int c;
    fp = fopen("f2.txt", "r");
    if (fp == NULL) {
        printf("File doesn't exist!\n");
        exit(1);
    }
    printf("File successfully opened\n");
    
    while(1) {
        c = fgetc(fp); // to read a character from a file
        if ( c != EOF ) {
            printf("%c",c);
        }
        else {
            break;
        }
    }
    
    // better way
    /*while( (  c = fgetc(fp) )  != EOF  )  {
       printf("%c", c);
    }*/
    
    printf("File successfully read\n");
    fclose(fp);
    return 0;
}