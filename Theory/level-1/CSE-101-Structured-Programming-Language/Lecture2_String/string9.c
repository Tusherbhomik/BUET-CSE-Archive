#include<stdio.h>

// mystrcmp(s,t) - compares s and t, if equal returns 0, otherwise returns the difference of first mismatch
int mystrcmp(char t[], char s[]) {
    int i = 0;
	while(s[i]  && t[i] && s[i]==t[i] ){
		i++;
	}
	return s[i]-t[i];
}

// mystrncmp(s,t,n) - compares s and t upto n chars, if equal returns 0, otherwise returns the difference of first mismatch
int mystrncmp(char t[], char s[], int n) {
    int i = 0;
	while(s[i]  && t[i] && s[i]==t[i] && i < n){
		i++;
	}
    if ( i == n ) return 0;
    return s[i]-t[i];
}

int main() {
    char s[] = "hello world";
    char t[] = "hello";
    int cmp = mystrcmp(s,t);
    printf("Comparing %s and %s : %d\n", s, t, cmp);
    cmp = mystrncmp(s,t, 3);
    printf("Comparing %s and %s : %d\n", s, t, cmp);   
    return 0;
}