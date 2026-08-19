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

bool checkConnectivity(vector<int>adj[],int V)
{
    bool visited[V];
    memset(visited,false,sizeof(visited));

    DFS(0,adj,visited);
    for(int i=0;i<V;i++)
    {
        if(!visited[i])return false;
    }
    return true;
}

int main()
{
    freopen("in.txt","r+",stdin);
    freopen("Reverse_Del_Algo.txt","w",stdout);

    int n,m;
    cin>>n>>m;

    vector<pair<double,pair<int,int>>>adj;//for every edge
    vector<int> baseADJ[n];
    vector<pair<int,int>> ans;

    int u,v,d;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v>>d;
        adj.push_back(make_pair(d,make_pair(u,v)));
        baseADJ[u].push_back(v);
        baseADJ[v].push_back(u);
    }

    sort(adj.begin(),adj.end(),greater<pair<double,pair<int,int>>>());
    /*for(int i=0;i<m;i++)
    {
        cout<<adj[i].second.first<<" "<<adj[i].second.second<<" "<<adj[i].first<<endl;
    }*/

    bool deletedEdge[m];
    memset(deletedEdge,false,sizeof(deletedEdge));

    for(int i=0;i<m;i++)
    {
        deletedEdge[i]=true;// true means i deleted/ or i can delete
        int x=adj[i].second.first;
        int y=adj[i].second.second;
        baseADJ[x].erase(remove(baseADJ[x].begin(), baseADJ[x].end(),y), baseADJ[x].end());
        baseADJ[y].erase(remove(baseADJ[y].begin(),baseADJ[y].end(),x),baseADJ[y].end());
        if(!checkConnectivity(baseADJ,n))//if check connectivity returns false that means graph becomes disconnected
        {
            deletedEdge[i]=false;
            baseADJ[x].push_back(y);
            baseADJ[y].push_back(x);
        }

        /*for(int i=0;i<n;i++)
        {

            for(int u: baseADJ[i])cout<<u<<" ";
            cout<<endl;
        }*/
    }
     double weight=0;
    for(int i=0;i<m;i++)
    {
        if(!deletedEdge[i])
        {
            weight+=adj[i].first;
            ans.push_back(make_pair(adj[i].second.first,adj[i].second.second));
        }
    }
    cout<<weight<<endl;

    for(int i=0;i<ans.size();i++)
    {
        cout<<ans[i].first<<" "<<ans[i].second<<endl;
    }

}
