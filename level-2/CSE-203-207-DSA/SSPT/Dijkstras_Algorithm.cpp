#include<bits/stdc++.h>
using namespace std;
#define pb push_back

bool compair(const pair< pair<int,int>,double> &a, const pair< pair<int,int>,double> &b)
{
    if (a.second > b.second)
    {
        return true;
    }
    else if (a.second == b.second)
    {
        return false;
    }
    else
    {
        return false;
    }
}
void print(int parent [],int source,int destination,int ult_Des)
{   
    if(destination>source)
        print(parent,source,parent[destination],ult_Des);
    cout<<destination;
    if(destination!=ult_Des)
    cout<<"->";
}
int main()
{
    freopen("out.txt","w",stdout);
    freopen("in.txt","r+",stdin);
    int n,m,u,v,source,destination,currV;
    cin>>n>>m;
    double w;
    double distance[n];
    
    int parent[n];
    memset(parent,-1,sizeof(parent));//O(V)
    vector<pair<int,double>>adj[n];
    bool sptSet[n];
    memset(sptSet,false,sizeof(sptSet));
    
    for(int i=0;i<n;i++)
    {
        distance[i]=DBL_MAX;
    }
    for(int i=0;i<m;i++)
    {
        cin>>u>>v>>w;
        adj[u-1].pb({v-1,abs(w)});
    }
    cin>>source>>destination;
    source--,destination--;
    priority_queue< pair< pair<int,int>,double>,vector<pair< pair<int,int>,double>>,decltype(&compair)> pq(compair);
    distance[source]=0;
    sptSet[source]=true;
    for ( pair<int,double> u: adj[source])
    {
        pq.push({{source,u.first},u.second});
        distance[u.first]=u.second;
    }//O(E)

    while (!pq.empty())
    {
        currV=pq.top().first.second;
        
        if(!sptSet[currV]){
        parent[currV]=pq.top().first.first;
        sptSet[currV]=true;
        pq.pop();
        for(pair<int,double> u: adj[currV])
        {
            distance[u.first]=min(distance[currV]+u.second,distance[u.first]);
            //cout<<currV<<" "<<u.first<<" "<<distance[u.first]<<endl;
           pq.push({{currV,u.first},distance[u.first]});
        }
        }
        else pq.pop();
    }
    cout<<"Dijkstra Algorithm:\n"<<distance[destination]<<endl;

    //printing
    /*stack<int> path;
    path.push(destination);
    do
    {
        path.push(parent[destination]);
        destination=parent[destination];
        
    } while (destination!=source);

    while (!path.empty())
    {
        cout<<path.top();
        path.pop();
        if(!path.empty())
        cout<<"->";
    }*/
    
   print(parent,source,destination,destination);
}















