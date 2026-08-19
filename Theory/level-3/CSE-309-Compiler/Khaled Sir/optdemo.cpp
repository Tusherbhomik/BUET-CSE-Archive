//------Links-----------------------------------------------------------------
GCC Optimization: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
Link of godbolt lecture: https://www.youtube.com/watch?v=w0sz5WbS5AM&t=331s
A writing on optimization: https://queue.acm.org/detail.cfm?id=3372264
Link of Compiler Explorer: https://godbolt.org/

//------Sample Codes for Illustrating Optimizations by GCC--------------------
//algebric identities; change values of multiplier
int a,b,c,d,e, w,x,y,z,flag;
int main(){
   a=x*1;
   b=y*2;    
   c=z*3;
   d=x/3;
   e=y%3;
   return 0;
}

//application of commutativity; see the difference between clang and gcc
int a,b,w,x,y;
int main(){
    a=w+x;
    b=x+y+w;
}

//dead code elimination;
int a,b,w,x,y,z,flag;
int main(){
    flag=1;
    if(flag) a=w+x;
    else a=y+z;
    return 0;
}

int a,b,w,x,y,z,flag;
int main(){
    flag=1;
    a = x>y && (y<z || flag);
    return 0;
}

//Constant unfolding; see how all the induction vars changed
#define MCONST 3
int a[100];
int size=5;
int main(){
    for(int i=0;i<12;i++){
        a[i]=2*MCONST*i;
    }
}

//Loop unfolding
#include<stdio.h>
int a[10];
int main(){
    for(int i=0;i<9;i++){ //keep changing the values of i to see when it stops
        a[i]=i;
    }
}

//Tail recursion. Use O1 and then O2 switches
#pragma GCC optimize ("O2")
#include<stdio.h>
int seriesSum(int n)
{
    if (n == 1)  return 1;    
    return n+seriesSum(n-1);
}
int main(){
    int i,n;
    i=seriesSum(5);
    printf("%d",i);
}

//Others
int a,b;
int main(){
    for (int i=0;i<10;i++){
        for (int j=0;j<10;j++){
			b=a; //add this later and in o3; does not work in o2
            a=i*10+j;
        }
    }
    return 0;
}




