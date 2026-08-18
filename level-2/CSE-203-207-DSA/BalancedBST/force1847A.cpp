#include <iostream>
#include<vector>
#include<algorithm>
using namespace std;
void solve()
{
    int n,k,a;
    cin>>n>>k;
    vector<int> sus,dif;
    for(int i=0;i<n;i++)
    {
        cin>>a;
        sus.push_back(a);
    }
    if(k==n)
    { 
        cout<<0<<endl;
        return;
    }
    
    for(int i=0;i<sus.size()-1;i++)
    {
        dif.push_back(abs(sus[i]-sus[i+1]));
    }

    sort(dif.begin(),dif.end());
    int sum=0;
    for(int i=0;i<=(dif.size()-k);i++)
    {
        sum+=dif[i];
    }
    cout<<sum<<'\n';

}
int main()
{

    freopen("out.txt","w",stdout);
    freopen("in.txt","r+",stdin);
    
    int tc;
    cin>>tc;
    for(int i=0;i<tc;i++)
    {
    solve();
    }
    return 0;
}
