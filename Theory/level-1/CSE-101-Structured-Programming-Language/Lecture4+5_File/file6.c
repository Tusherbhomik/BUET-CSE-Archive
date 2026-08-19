#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    char str[80];
    int i;
    float f;
    
    fp = fopen("f6.txt", "w"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    // printf("%d %d %d", a, 20, 30);
    fprintf(fp, "%f %s %d ", 100.5, "hello", 20); // to write in a file, similar as printf
    fprintf(fp, "%f %s %d ", 345.5, "world", 10);
    fclose(fp);
    
    fp = fopen("f6.txt", "r"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
    while (!feof(fp)) {
        fscanf(fp, "%f %s %d ", &f,  str,&i); // to read from a file, similar as scanf
        printf("%f %s %d\n", f, str, i);
    }
    fclose(fp);
    return 0;


    %d,&n

    N 
    [67]
    1000
}
&imh 
[sturc] 
10000