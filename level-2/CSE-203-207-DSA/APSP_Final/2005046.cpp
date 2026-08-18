#include <bits/stdc++.h>
using namespace std;

int main()
{
    //freopen("out.txt", "w", stdout);
    //freopen("in.txt", "r+", stdin);
    int n;
    cin>>n;
    string str;
    map<string,int>mp;
    float arr[n][n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if(i==j)
                arr[i][j]=1;
            else
                arr[i][j] = 0;
        }
    }

    for(int i=0; i<n; i++)
    {
        cin>>str;
        mp[str]=i;
    }
    int m;
    cin>>m;
    string a,b;
    float d;

    for(int i=0; i<m; i++)
    {
        cin>>a>>d>>b;
        arr[mp[a]][mp[b]] = d;
       // cout<<mp[a]<<" "<<mp[b]<<" "<<d<<endl;
    }

    /*for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                arr[i][j] = max(arr[i][j], arr[i][k] + arr[k][j]);
            }
        }
    }
    bool check=false;
    for(int i=0;i<n;i++)
    {

        for(int j=0;j<n;j++)
        if(arr[i][j]){
            check=true;
            break;
        }
    }
    if(check) cout<<"yes";
    else cout<<"false"<<endl;*/
    for (int i=0;i<n;i++){
    for(int j=0;j<n;j++)
    {
        cout<<arr[i][j]<<" ";
    }
    cout<<endl;
    }
    return 0;
}




