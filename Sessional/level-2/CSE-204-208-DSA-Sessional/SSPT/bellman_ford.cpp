#include <bits/stdc++.h>
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
void print(int parent[], int source, int destination, int ult_Des)
    {
        if (destination > source)
            print(parent, source, parent[destination], ult_Des);
        cout << destination;
        if (destination != ult_Des)
            cout << " - >";
    }
void Bellman_ford(vector<pair<pair<int, int>, double>> edges,int n,int m,int source,int destination)
{
    double previous,distance[n];
    bool hasNegativeCycle = false;
    int parent[n];
    memset(parent, -1, sizeof(parent));
    for (int i = 0; i < n; i++)
    {
        distance[i] = DBL_MAX;
    }
    distance[source] = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // if(distance[edges[j].first.second]> distance[edges[j].first.first] + edges[j].second)
            // {
            //     parent[edges[j].first.second]=edges[j].first.first;
            // }
            distance[edges[j].first.second] = min(distance[edges[j].first.second], distance[edges[j].first.first] + edges[j].second);
        }
    }
    // negative cycle check
    
    // for (int j = 0; j < m; j++)
    // {
    //     previous = distance[edges[j].first.second];
    //     distance[edges[j].first.second] = min(distance[edges[j].first.second], distance[edges[j].first.first] + edges[j].second);
    //     if (previous != distance[edges[j].first.second])
    //     {
    //         hasNegativeCycle = true;
    //         break;
    //     }
    // } 
    cout << "Bellman Ford Algorithm:\n";
    if (!hasNegativeCycle)
    {
        cout << distance[destination] << endl;
        // path show
        print(parent, source, destination, destination);
        cout<<endl;
    }
    else
        cout << "Negative weight cycle present\n" << endl; 
}
void Dijkstra_algorithm(vector<pair<pair<int, int>, double>> edges,int n,int m,int source,int destination)
{
    int parent[n],currV;
    memset(parent, -1, sizeof(parent));
    bool sptSet[n];
    memset(sptSet,false,sizeof(sptSet));//O(V)
    double distance[n];
    vector<pair<int,double>>adj[n];

    for (int i = 0; i < n; i++)
    {
        distance[i] = DBL_MAX;
    }
    for(int i=0;i<m;i++)
    {
        adj[edges[i].first.first].pb({edges[i].first.second,abs(edges[i].second)});
    }

    //algo start
    distance[source] = 0;
    sptSet[source]=true;
    priority_queue< pair< pair<int,int>,double>,vector<pair< pair<int,int>,double>>,decltype(&compair)> pq(compair);//O(1)
    sptSet[source]=true;
    for ( pair<int,double> u: adj[source])
    {
        pq.push({{source,u.first},u.second});
        distance[u.first]=u.second;
    }

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
   cout<<endl;
}

int main()
{
    freopen("out.txt", "w", stdout);
    freopen("in.txt", "r+", stdin);

    //Input
    //_________________________________
    int n, m, u, v, source, destination;
    double w;
    cin >> n >> m;
    vector<pair<pair<int, int>, double>> edges;
    for (int i = 0; i < m; i++)
    {
        cin >> u >> v >> w;
        edges.pb({{u, v}, w});
    }
    cin >> source >> destination;
    //______________________________________

    Bellman_ford(edges,n,m,source,destination);
    Dijkstra_algorithm(edges,n,m,source,destination);
}