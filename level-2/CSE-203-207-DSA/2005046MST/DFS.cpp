#include<bits/stdc++.h>
using namespace std;

void DFS(int u,vector<int>adj[],bool visited[])
{
    visited[u]=true;
    for(int v: adj[u])
    {
        if(!visited[v])
        DFS(v,adj,visited);
    }
}

int main()
{
    int n,m;
    cin>>n>>m;
    bool visited[n];
    memset(visited,false,sizeof(visited));
    vector<int> adj[n];
    int x,y;
    for(int i=0;i<m;i++)
    {
        cin>>x>>y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    for(int i=0;i<n;i++)
    {
        if(!visited[i])DFS(i,adj,visited);
    }

}
