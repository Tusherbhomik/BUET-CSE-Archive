#include<stdio.h>
#include<stdlib.h>


int main() {
    FILE * fp;
    fp = fopen("f1.txt", "w"); // to open file in read mode
    //fp = fopen("f1.txt", "w"); // to open file in write mode
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    printf("File successfully opened\n");
    fclose(fp);
    return 0;
}
