#include<stdio.h>
#include<string.h>

// mystrfind(s,t) - find t anywhere in s
/*int mystrfind(char s[], char t[]) {
    int x = strlen(s);
    int y = strlen(t);
    int i, j= 0;
    for ( i = 0; i < x; i++) {
        if (s[i] == t[j]) {         // character match
            j++;                         // advance t
        }
        else {                          // character mismatch
            i-=j;                       // reset s by removing existing matches
            j = 0;                       // reset t with 0
        }
        if ( j == y ) return 1;
    }   
    return 0;
}*/

// mystrfind2(s,t) - find t anywhere in s
int mystrfind2(char s[], char t[])
{
	 int i = 0, j = 0, start = 0;
	  while (s[i])  {
          start = i;
           j = 0; 
           while (s[i] && t[j] && s[i] == t[j]) {
                    i++;
                    j++;
            }
			if (!t[j]) {
                return start;
            }
		    i  = start + 1;	
	  }
	  return -1;
}

int main() {
    char s[80], t[80];
    printf("Enter 1st string: ");
    gets(s);
    printf("Enter 2nd string: ");
    gets(t);
    int cmp = mystrfind(s, t); 
    printf("Finding %s in %s : %d\n", t, s, cmp);
    return 0;
}
