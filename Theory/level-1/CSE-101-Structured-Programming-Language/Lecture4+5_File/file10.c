#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    FILE * fp;
    long loc; 
    fp = fopen("file10.c", "rb"); 
     if (fp == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
   printf("Enter a location: ");
   scanf("%ld", &loc);
   
   // SEEK_SET from start, SEEK_END from end, SEEK_CUR from current
   // minus value of loc is valid with SEEK_END and SEEK_CUR
   if (fseek(fp, loc, SEEK_SET)) {
       printf("File Seek Error");
       exit(1);
   }
   
   printf("Value at loc %ld is %c\n", loc, fgetc(fp));
   




//    fseek(fp, 0, SEEK_END);
//    long size = ftell(fp); // tell the current bytes 
//    printf("Size of the file: %ld\n", size);
   
   fclose(fp);
   return 0;
}