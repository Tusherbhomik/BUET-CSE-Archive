#include<stdio.h>
#include<string.h>

int main() {
    char s[80], t[80]; // declare two empty string with capacity 80 
    char u[80] = "C string ";
    int len, cmp;
    printf("Enter your string: ");
    gets(s);


    // strcpy(t,s) - copies content of s to t, overwrite t
    strcpy(t,s); 
    printf("Your copied string: %s\n", t);





    // strcat(u,t) - concats content of t to u, doesn't overwrite u
    strcat(u,t); 
    printf("Your concatenated string: %s\n", u);




    // strlen(u) - returns the length of u
    len = strlen(u); 
    printf("Length of your concatenated string: %d\n", len);



    // strcmp(s,t) - compares s and t, if equal returns 0, otherwise returns the difference of first mismatch
    cmp = strcmp(s,t); 
    printf("Comparing %s and %s : %d\n", s, t, cmp);
    cmp = strcmp(u,t);
    printf("Comparing %s and %s : %d\n", u, t, cmp);




    // you can also use fixed strings as second parameter
    strcpy(u, "hello");
    strcat(u, " world");
    len = strlen("hello world");
    printf("New string = %s, length = %d\n", u, len);    
    return 0;
}