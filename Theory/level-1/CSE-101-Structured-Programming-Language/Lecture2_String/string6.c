#include<stdio.h>

// mystrcpy(t,s) - copies content of s to t, overwrite t 
void mystrcpy(char t[], char s[]) {
    int i = 0;
    while(s[i] != '\0') {
        t[i] = s[i];
        i++;
    }
    t[i] = '\0';
}


// mystrlen(s) - returns the length of s
int mystrlen(char s[]) {
    int i = 0;
    while(s[i] != '\0') {
        i++;
    }
    return i;
}



// mystrcat(t,s) - concats content of s to t, doesn't overwrite t
void mystrcat(char t[], char s[]) {
    int i = 0;
    int j = mystrlen(t);
    while(s[i] != '\0') {
        t[j] = s[i];
        i++;
        j++;
    }
    t[j] = '\0';
}


// mystrcmp(s,t) - compares s and t, if equal returns 0, otherwise returns the difference of first mismatch
int mystrcmp(char s[], char t[]) {
    int i = 0;
	while(s[i]!='\0' && t[i]!='\0' && s[i]==t[i] ){
		i++;
	}
	return s[i]-t[i];
}

int main() {
    char s[80], t[80];
    char u[80] = "C string ";
    int len, cmp;
    printf("Enter your string: ");
    gets(s);
    mystrcpy(t,s);
    printf("Your copied string: %s\n", t);
    mystrcat(u,t);
    printf("Your concatenated string: %s\n", u);
    len = mystrlen(u);
    printf("Length of your concatenated string: %d\n", len);
    cmp = mystrcmp(s,t);
    printf("Comparing %s and %s : %d\n", s, t, cmp);
    cmp = mystrcmp(u,t);
    printf("Comparing %s and %s : %d\n", u, t, cmp);   
    return 0;
}