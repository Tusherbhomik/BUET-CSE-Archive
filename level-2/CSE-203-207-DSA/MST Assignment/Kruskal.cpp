#include<bits/stdc++.h>
using namespace std;

bool compare(const pair<double, pair<int,int>>& a, const pair<double, pair<int,int>>& b) {
    return a.first < b.first;
}
void make_set(int v,int parent[]) {
    parent[v] = v;
}
int find_set(int v,int parent[]) {
    if (v == parent[v])
        return v;
    return parent[v] =find_set(parent[v],parent);
}

void union_sets(int a, int b,int parent[]) {
    a = find_set(a,parent);
    b = find_set(b,parent);
    if (a != b)
        parent[b] = a;
}
int main()
{
    freopen("in.txt","r+",stdin);
    freopen("Kruskal.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    vector<pair<double,pair<int,int>>>adj;
    vector<pair<int,int>> ans;
    int u,v,d;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v>>d;
        adj.push_back(make_pair(d,make_pair(u,v)));
    }
    sort(adj.begin(),adj.end(),compare);
    /*for(int i=0;i<m;i++)
    {
        cout<<adj[i].second.first<<" "<<adj[i].second.second<<" "<<adj[i].first<<endl;
    }*/
    double cost=0;
    int parent[n];
    for(int i=0;i<n;i++)
    {
        make_set(i,parent);
    }
    for(int i=0;i<m;i++)
    {
        //check cycle create or not ?
        if(find_set(adj[i].second.first,parent)!=find_set(adj[i].second.second,parent))
        {
            cost+=adj[i].first;
            union_sets(adj[i].second.first,adj[i].second.second,parent);

            ans.push_back(make_pair(adj[i].second.first,adj[i].second.second));
        }
        //if not then add cost then goes down

    }
    cout<<"Kruskal’s Algorithm:"<<endl;
    cout<<"Total Weight = "<<cost<<endl;
    for(int i=0;i<ans.size();i++)
    {
        cout<<ans[i].first<<" "<<ans[i].second<<endl;
    }
     return 0;

}




