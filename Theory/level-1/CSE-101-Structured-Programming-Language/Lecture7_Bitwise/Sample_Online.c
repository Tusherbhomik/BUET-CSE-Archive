#include <stdio.h>

void printbits(int n)
{
    int mask=1<<31;
    int count_1=0;
    for(int i=0;i<32;i++)
    {
        if (i!=0 && i%4==0)
            printf(" ");
        if (n&mask)
        {   
            printf("1");
            count_1++;
        }
        else
            printf("0");
        n=n<<1;    
    }
    printf(" : %d\n", count_1);
}
int ax=15;
//10,2->14
int setBit(int n, int b)
{               //n=0000 0000 0000 0000 0000 0000 0000 1010
    unsigned int mask=1; //mask=0000 0000 0000 0000 0000 0000 0000 0001 
    mask=mask<<b;//mask=0000 0000 0000 0000 0000 0000 0000 0100
    return n|mask;
}
//14,2->10
int resetBit(int n, int b)
{
    unsigned int mask=1;
    mask=mask<<b;//mask=0000 0000 0000 0000 0000 0000 0000 0100
    mask=~mask;
    return n&mask;
}





int justLarger(int n)
{
    int m=n;
    unsigned int mask_0=1,mask_1=2;
    int cnt_0=0,cnt_1=0;
    int i=0;
    for(i=0;i<31;i++)
    {
        if((mask_0&n)==1)
        {
            if((mask_1&n)==0)
            {
                printf("we got it\n");
                unsigned int result;
                result=setBit(m,i+1);
                result=resetBit(result,i);
                int j;
                //printf("i=%d,cnt_0=%d,cnt_1=%d\n",i,cnt_0,cnt_1);
                for(j=0;j<cnt_1;j++)
                {
                    result=setBit(result,j);
                }
                for(j=0;j<cnt_0;j++)
                {
                    result=resetBit(result,cnt_1+j);
                }
                return result;
                
            }
            cnt_1++;
        }
        else
            cnt_0++;

    printbits(n);
    n=n>>1;
    }
    return 0;
}

//// Just a different implementation. Dont try much to understand that. :p 

// int justLarger(int n)
// {
//     int m=n;
//     unsigned int mask_0=1,mask_1=2;
//     int cnt_0=0,cnt_1=0;
//     int i=0;
//     for(i=0;i<31;i++)
//     {
//         if((mask_0&n)==1)
//         {
            
//             if((mask_1&n)==0)
//             {

//                 //printf("we got it\n");
//                 unsigned int result;
//                 result=setBit(m,i+1);
//                 result=resetBit(result,i);
//                 int j;
//                 printf("i=%d,cnt_0=%d,cnt_1=%d\n",i,cnt_0,cnt_1);
//                 int min=cnt_0>cnt_1?cnt_1:cnt_0;
//                 int max=cnt_0+cnt_1-min;
//                 unsigned int mask= ((1<<min)-1)<<max|((1<<min)-1);
//                 printbits(mask);
//                 result=result ^ mask; 

//                 return result;
                
//             }
//             cnt_1++;

//         }
//         else
//             cnt_0++;

//     printbits(n);
//     n=n>>1;
//     }
//     return 0;
// }

int main()
{
    int a=10;
    int b=-10;
    printbits(a);
    printbits(b);
    printf("%d\n", setBit(10,2));
    printf("%d\n", setBit(10,3));
    printf("%d\n", resetBit(14,2));
    printf("%d\n", resetBit(14,3));
    printbits(justLarger(0b00000000011111));
    printf("%d\n", justLarger(412));

}

