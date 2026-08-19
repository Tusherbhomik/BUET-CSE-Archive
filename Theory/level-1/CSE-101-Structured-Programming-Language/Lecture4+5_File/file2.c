#include<stdio.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    char str[] = "This is a file system test.\n";
    int i = 0;
    fp = fopen("f2.txt", "w");
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    printf("File successfully opened\n");
    
    while(str[i]) {
        int ret = fputc(str[i], fp); // to write a character in a file
        if ( ret == -1) {  // EOF = -1
            printf("Error writing file!\n");
            exit(1);
        }
        i++;
    }
    
    // better way
    /*while (str[i] && fputc(str[i] , fp) != EOF) {
        i++;
    }*/
    
    printf("File successfully written\n");
    fclose(fp);
    return 0;
}