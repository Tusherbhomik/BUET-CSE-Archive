#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    int N = 10;
    int i = 0;
    int a[] = {1, 3, 5, 7, 9, 0, 2, 4, 6, 8};
    
    fp = fopen("f8", "wb"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
        
    //if ( fwrite(a, sizeof(a), 1, fp) != 1)    
    if ( fwrite(a, sizeof(int), N, fp) != N ) //will work too
    {         
        printf("Write Error\n");
        exit(1);
    }
    fclose(fp);
    
    for (i = 0; i < N; i++) a[i] = 0;
   
    fp = fopen("f8", "rb"); 
    if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
    // if ( fread(a, sizeof(a), 1, fp) != 1) 
    if ( fread(a, sizeof(int), N-2, fp) != N-2 )
    {
        printf("Read Error\n");
        exit(1);
    }
    fclose(fp);
    
    for (i = 0; i < N-2; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    
    return 0;
}