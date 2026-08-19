#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    char str[80];
    int N = 5;
    int i;
        
    fp = fopen("f5.txt", "a"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
    printf("Enter %d lines to save in a file:\n", N);
    
    for ( i = 0; i < N; i++) {
        gets(str);
        strcat(str, "\n"); // need to append the '\n' because fgets use it as terminator
        fputs(str, fp); // to write a string in a file
    }
    fclose(fp);
    
    fp = fopen("f5.txt", "r"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }

    printf("Your lines:\n");
    while(!feof(fp)) {
        fgets(str, 80, fp); // to read a string from a file
        if (!feof(fp)) printf("%s", str);
    }
    fclose(fp);
    return 0;
}


int a(int b, int c)
{
    return 0;
}
