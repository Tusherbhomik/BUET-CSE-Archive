#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    int indicator=0;
    fp=fopen("in.txt","r");
    while(!feof(fp)) {
        fgetc(fp);
        indicator++;
    }
    printf("OUTPUT: %d\n",indicator);
}