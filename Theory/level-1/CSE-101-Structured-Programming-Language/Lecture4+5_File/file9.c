#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    int M = 3, N = 4;
    int i = 0, j =0;
    int a[3][4] = {1, 3, 5, 7, 9, 0, 2, 4, 6, 8, 1, 2};
    fp = fopen("f9", "wb"); 
    fwrite(a, sizeof(a), 1, fp); //  fwrite(a, sizeof(int), M * N, fp) will work too
    fclose(fp);
    
    for (i = 0; i < M; i++) {
       for (j = 0; j < N; j++) {
                a[i][j] = 0;
       }
    }
   
    fp = fopen("f9", "rb"); 
    fread(a, sizeof(a), 1, fp);
    fclose(fp);
    
    for (i = 0; i < M; i++) {
       for (j = 0; j < N; j++) {
                printf("%d ", a[i][j]);
       }
       printf("\n");
    }
    
    return 0;
}