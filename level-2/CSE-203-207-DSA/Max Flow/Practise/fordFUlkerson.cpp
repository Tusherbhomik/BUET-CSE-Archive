#include <bits/stdc++.h>
using namespace std;
#define inf 100000
int n;

int bfs(int s,int t ,vector<int> &parent,vector<vector<int>>&capacity,vector<vector<int>> &adj)
{
    fill(parent.begin(),parent.end(),-1);
    queue<pair<int,int>> q;
    parent[s]=-2;
    q.push({s,inf});
    
    while(!q.empty())
    {
        
        int curr=q.front().first;
        int flow=q.front().second;

        q.pop();
        for(int next: adj[curr])
        {
            if(parent[next]=-1 && capacity[curr][next])
            {
                parent[next]=curr;
                int new_flow=min(flow,capacity[curr][next]);
        
                if(curr==t) 
                {
                    cout<<new_flow<<endl;
                    return new_flow;
                }
                q.push({next,new_flow});
            }
        }
    }
    return 0;


}

void ford_fulkarson(int s,int t,vector<vector<int>>&capacity,vector<vector<int>> &adj)
{
    
    vector<int> parent(n);
    int flow=0;
    int new_flow;
    
    while(new_flow=bfs(s,t,parent,capacity,adj))
    {
        cout<<new_flow<<endl;
        flow+=new_flow;
        int curr=t;
        while (curr!=s)
        {
            int prev=parent[curr];
            capacity[curr][prev]+=new_flow;
            capacity[prev][curr]-=new_flow;
            curr=prev;
        }
    }
    cout<<"flow is : "<<flow<<endl;
}
int main()
{

    freopen("out.txt","w",stdout);
    freopen("in.txt","r+",stdin);

    int e;
    cin>>e;
    n=e;
    int u,v,c;
    vector<vector<int>>capacity(5,vector<int>(5,0));
    vector<vector<int>>adj(5,vector<int>(0));
    for(int i=0;i<e;i++)
    {
        cin>>u>>v>>c;
        adj[u].push_back(v);
        capacity[u][v]=c;
       
    }
    
    int s,t;
    cin>>s>>t;

    ford_fulkarson(s,t,capacity,adj);
    return 0;
}
