#include<bits/stdc++.h>
using namespace std;

int main()
{
    // freopen("in.txt","r",stdin);
    // freopen("out.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    int root=0;
    bool sptSet[n];
    double distance[n];
    for(int i=0;i<n;i++)
    {
        distance[i]=DBL_MAX;
    }
    vector<pair<int,double>>adj[n];//vector of pair <destination,weight>
    vector<pair<int,int>>ans;
    int u,v,d;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v>>d;
        adj[u].push_back({v,d});
        adj[v].push_back({u,d});
    }
    bool visited[n];
    memset(visited,false,sizeof(visited));
}

