#include<stdio.h>
#include<stdlib.h>

int main() {
    FILE * from;
    FILE * to;
    int c;
    char src[80], dst[80];
    printf("Enter source file name: ");
    scanf("%s", src);
    printf("Enter destination file name: ");
    scanf("%s", dst);
    
    from = fopen(src, "rb"); //  rb for binary
    if (from == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
    
    to = fopen(dst, "a"); //  wb for binary
    if (to == NULL) {
        printf("File doesn't exists!\n");
        exit(1);
    }
   
    // without any error checking
    // while( (  c = fgetc(from) )  != EOF  )  {
    //    fputc(c, to);
    // }
    
    // with error checking 
    //feof = true => for two reasons: 1) error OR 2)EOF (End Of File)
    while(!feof(from)) { 
        c = fgetc(from);
        if (ferror(from)) {
            printf("From File Error\n");
            break;
        }
        if (!feof(from)) { 
            fputc(c, to);
        }
        if (ferror(to)) {
            printf("To File Error\n");
            break;
        }
     }
     
    fclose(from);
    fclose(to);
    return 0;
}