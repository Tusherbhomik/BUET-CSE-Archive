#include <stdio.h>

//1000 0000 0000 0000 0000 0000 0000 0000
//1000 0000 0100 0100 0000 0000 0000 0000
void printBits(int x)
{
    int i;
    int mask = (1 << 31);
    for (i = 0; i < 32; i++)
    {
        if (i%4==0)
           printf(" ");
        printf("%d", ((x & mask) ? 1 : 0));
        x <<= 1;
    }
    printf("\n");
}

// 0000 0000 0000 0000 0000 0000 0000 1100
// 0000 0000 0000 0000 0000 0000 0001 1001
// 0000 0000 0000 0000 0000 0000 0000 1000
// 0000 0000 0000 0000 0000 0000 0001 0101
// 0 1 / 1 0 ->1
// 0 0 / 1 1 ->0


int main()
{
    int a = 111112, b = 215;
    printf("a and b = %x\n", a&b);
    printf("a or b = %d\n", a|b);
    printf("a xor b = %d\n", a^b);
    printf("not a = %d\n", ~a);
    printf("not b = %d\n", ~b);



    printBits(a);
    printBits(b);
    printBits(a&b);
    printBits(a|b);
    printBits(a^b);
    printBits(~b);



// X= 0/1
// 1 & X = X 
// 0 & X = 0
// ---
// 1 | X = 1 
// 0 | X = X
// --
// 1 ^ X = ~X
// 0 ^ X = X

// 1010
// 0100

// 0000 0000 0000 0000 0000 0000 0000 0100
    int c=101010;
    for (int i=1;i<=10;i++)
    {
        printf("%d\n",c);
        printBits(c);
        c=c>>2;
        //printf("%d\n",c);
        //printBits(c);
    }

    return 0;
}