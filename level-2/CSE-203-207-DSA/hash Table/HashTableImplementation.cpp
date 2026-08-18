#include<iostream>
using namespace std;

int main()
{
    int n;
    cout<<"Enter the size of hash table: ";
    cin>>n;
    int hashTable[n];
    for(int i=0;i<n;i++)
        hashTable[i]=-1;
    int key;
    cout<<"Enter the key to be inserted: ";
    cin>>key;
    int index=key%n;
    if(hashTable[index]==-1)
        hashTable[index]=key;
    else
    {
        int i=1;
        while(1)
        {
            int newIndex=(key+i)%n;
            if(hashTable[newIndex]==-1)
            {
                hashTable[newIndex]=key;
                break;
            }
            i++;
        }
    }
    cout<<"Hash Table: ";
    for(int i=0;i<n;i++)
        cout<<hashTable[i]<<" ";
    cout<<endl;
    return 0;
}