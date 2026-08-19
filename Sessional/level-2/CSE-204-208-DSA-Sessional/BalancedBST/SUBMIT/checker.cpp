#include<bits/stdc++.h>
using namespace std;
int main(){
    ifstream file1("out.txt");
    ifstream file2("properout.txt");
    string line1,line2;
    int i=1;
    while(getline(file1,line1)){
        getline(file2,line2);
        if(line1!=line2){
            cout<<"no";
            cout<<i;
            return 0;
        }
        i++;
    }
    cout<<"yes";
    return 0;
}