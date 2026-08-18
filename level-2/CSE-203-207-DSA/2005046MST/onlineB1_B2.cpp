#include<bits/stdc++.h>
using namespace std;
bool compair(const pair<int,pair<int,int>> &a, const pair<int,pair<int,int>> &b)
{
    if (a.second.second > b.second.second)
    {
        return true;
    }
    else if (a.second.second == b.second.second)
    {
        return false;
    }
    else
    {
        return false;
    }
}
int main()
{
    //freopen("in.txt","r",stdin);
    //freopen("online.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    vector<pair<int,int>>adj[n+1];//i= source ,<destination,weight>
    int a,b,c;
    for(int i=1;i<=m;i++)
    {
        cin>>a>>b>>c;
        adj[a].push_back({b,c});
        adj[b].push_back({a,c});
    }
    priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,decltype(&compair)>pq(compair);//weight,source,destination
    bool menVisited[n];
    bool womenVisited[n];
    memset(menVisited,false,sizeof(menVisited));
    memset(womenVisited,false,sizeof(womenVisited));
    for( pair<int,int> u: adj[1])
    {
        pq.push({u.second,{1,u.first}});
    }
    cout<<"Here Goes..."<<endl;
    while(!pq.empty())
    {
        cout<<pq.top().second.first<<" "<<pq.top().second.second<<" "<<pq.top().first<<endl;
        pq.pop();
    }

}
