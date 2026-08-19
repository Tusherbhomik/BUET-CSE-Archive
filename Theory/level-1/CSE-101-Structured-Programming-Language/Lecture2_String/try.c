#include<stdio.h>
#include<stdlib.h>


    void  mystring_find(char s[],char t[]){
        int i=0,j=0,start=0;
        while(t[i]){
            while(s[j]==t[i]&& s[j]!='\0'){
                i++;
                j++;
            }
            }
            if(s[j]=='\0'){
                printf("NO");
                exit(0);
            }
            else i=0;
            if(!t[i]){
                printf("YES");
            
            }
           



    int main(){
        char s[100],t[100];
        printf("enter the string in which u want to find another string:");
        gets(s);
        printf("enter the small string:");
        gets(t);
        mystring_find(s,t);
        return 0;
    }