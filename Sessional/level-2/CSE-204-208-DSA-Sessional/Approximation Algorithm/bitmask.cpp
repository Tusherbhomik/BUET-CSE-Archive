//you are given a set represent as a bitmask u have to add,remove ,display the set

#include<bits/stdc++.h>
using namespace std;

void display(int subset)
{
    for(int bit_no=0;bit_no<=9;bit_no++)
    {
        if((subset&(1<<bit_no))!=0)
        {
            cout<<bit_no+1<<" ";
        }
    }
}
void add(int& subset,int i)
{
    subset=subset^(1<<(i-1));
}
void remove(int &subset,int i)
{
    subset=subset^(1<<(i-1));
}
//and your number with a number whoose i bit is 1 and all other bits are zero

int main()
{
    int set=15;
    remove(set,2);//xor operation
    remove(set,4);
    add(set,5);
    add(set,7);
   display(set);
    return 0;
}

