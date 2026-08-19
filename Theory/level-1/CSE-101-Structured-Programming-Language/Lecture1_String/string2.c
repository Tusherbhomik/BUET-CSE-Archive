#include<stdio.h>

int main() {
    char s[80]; // declare an empty string with capacity 80 
    printf("Enter first string: ");
    scanf("%s", s); // scan a single word
    //scanf("%s", &s[0]); // this will also work
    printf("You entered: %s\n", s);
    return 0;
}

