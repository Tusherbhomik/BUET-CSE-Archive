#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    while (1)
    {

        char a[10], b[10], c[10];
        int x, y, result;
        printf("Enter two numbers and operation (add/sub/mul/div): ");
        scanf("%s %s %s", a, b, c);
        // atoi(a) - converts string a into integer
        x = atoi(a);
        y = atoi(b);
        if (!strcmp(c, "add"))
            result = x + y; // if ( c == "add" ) doesn't work
        else if (!strcmp(c, "sub"))
            result = x - y;
        else if (!strcmp(c, "mul"))
            result = x * y;
        else if (!strcmp(c, "div"))
            result = y == 0 ? 0 : x / y;
        else
            result = 0;
        printf("Result: %d\n", result);
        
    }
    return 0;
}