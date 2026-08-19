#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    int i = 100;
    
    fp = fopen("f7", "wb"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
    if ( fwrite(&i, sizeof(int), 1, fp) != 1) { // to write any data type in binary mode to a file
        printf("Write Error\n");
        exit(1);
    }
    fclose(fp);
    
    i = 0;
    
    fp = fopen("f7", "rb"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
    if ( fread(&i, sizeof(int), 1, fp) != 1) { // to read any data type in binary mode from a file
        printf("Read Error\n");
        exit(1);
    }
    printf("i = %d\n", i);
    fclose(fp);
    return 0;
}