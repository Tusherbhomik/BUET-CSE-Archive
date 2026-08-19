#include <stdio.h>

void printBits(char *prefix, unsigned int x, char *suffix)
{
    int i;
    int nBits = sizeof(x)*8;
    unsigned int mask = ((unsigned int)1 << (nBits-1));
    printf("%s", prefix);
    for (i = 0; i < nBits; i++)
    {
        printf("%d", ((x & mask) ? 1 : 0));
        x <<= 1;
    }
    printf("%s", suffix);
}


//
// Swap 2 integers without any intermediate variable
// using bitwise operators
//
void swap(int *x, int *y)
{
    *x = *x ^ *y;   
    *y = *x ^ *y;   
    *x = *x ^ *y; 
}



//
// Extracts the higher order 16-bits of the input integer.
// Then prepares an integer with that pattern and returns it
//
unsigned int higher16(unsigned int x)
{
    unsigned int mask = ~0 << 16;

    x = (x & mask);
    x >>= 16;

    return x;
}


//
// Reverse a bit pattern and return
//
unsigned int bitsrev(unsigned int x)
{
    int n = sizeof(x) * 8;
    int i, j;
    unsigned int mask1 = 1;
    unsigned int mask2 = 1<<31;

    while (mask1 < mask2)
    {
        int i = ((x & mask1) != 0);
        int j = ((x & mask2) != 0);

        if (i^j != 0)
        {
            x = ((x ^ mask1) ^ mask2);
        }

        mask1 <<= 1;
        mask2 >>= 1;
    }

    return x;
}

int main()
{
    unsigned int x = 0;
    unsigned int y = 0;
    while(EOF != scanf("%u", &x))
    {
        y = higher16(x);
        printf("Integer constructed with higher order 16-bits: %u\n", y);
        printBits("Bit pattern:: ", y, "\n");
        printf("\n");

        y = bitsrev(x);
        printf("Integer constructed with bits reversed: %u\n", y);
        printBits("Bit pattern: ", y, "\n");
        printf("\n");

    }
    return 0;
}
