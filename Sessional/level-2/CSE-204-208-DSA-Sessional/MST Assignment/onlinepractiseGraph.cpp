#include<bits/stdc++.h>
using namespace std;
void topologicalSort(bool visited[],stack<int>&myStack,vector<int>Adj[],int i,int level[],int currLevel)
{
    visited[i]=true;
    for(int u: Adj[i])
    {
        if(!visited[u])
        {
            level[u]=currLevel;//2 and 3 er level same u hisabe 1 er janno 2 /3 ashbe 2 er dfs e jakhn dhukse
            //take ami or level baracchi other than that baracchi na , dfs e barano value niye dhukteseee
            // or dfs shes e 3 er dfs hbe r 3 er dfs korar shomoy 2 er dfs er current level e thakbe or kache

            level[u]++;
            cout<<u<<"--->"<<level[u]<<endl;
            topologicalSort(visited,myStack,Adj,u,level,level[u]);
        }
    }
    myStack.push(i);
}

int main()
{
    int n,m;
    cin>>n>>m;
    vector<int>Adj[n];
    int level[n];

    int x,y;
    for(int i=0; i<m; i++)
    {
        cin>>x>>y;
        Adj[x].push_back(y);
    }
    //Topological sort
    bool visited[n],dfsVisited[n];
    for(int i=0; i<n; i++)
    {
        visited[i]=dfsVisited[i]=false;
        level[i]=0;
    }
    stack<int> myStack;
    int GM;
    for(int i=0; i<n; i++)
    {
        if(!visited[i])
        {
            topologicalSort(visited,myStack,Adj,i,level,0);
            GM=myStack.top();
            cout<<GM<<" "<< myStack.size()-1<<endl;
            myStack.pop();
            while(!myStack.empty())
            {
             cout<<myStack.top()<<" "<<level[myStack.top()]<<endl;
             myStack.pop();
            }
            cout<<endl;


        }
    }
    return 0;
}
