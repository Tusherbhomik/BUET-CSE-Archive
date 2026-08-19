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
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int n;
    cin>>n;
    int x,y;
    vector<pair<int,int>> coordinates;
    vector<pair<double,pair<int,int>>>adj;
    for(int i=0;i<n;i++)
    {
        cin>>x>>y;
        coordinates.push_back({x,y});
    }
    double weight;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(i!=j){
            weight=abs(coordinates[i].first-coordinates[j].first)+abs(coordinates[i].second-coordinates[j].second);
            adj.push_back({weight,{i,j}});
            }
        }
    }
    /*for(int i=0;i<adj.size();i++)
    {
        cout<<adj[i].second.first<<" "<<adj[i].second.second<<" "<<adj[i].first<<endl;
    }*/
    sort(adj.begin(),adj.end());
    double cost=0;
    int parent[n];
    for(int i=0;i<n;i++)
    {
        make_set(i,parent);
    }
    for(int i=0;i<adj.size();i++)
    {
        //check cycle create or not ?
        if(find_set(adj[i].second.first,parent)!=find_set(adj[i].second.second,parent))
        {
            cost+=adj[i].first;
            union_sets(adj[i].second.first,adj[i].second.second,parent);

            //ans.push_back(make_pair(adj[i].second.first,adj[i].second.second));
        }
        //if not then add cost then goes down

    }
    cout<<"Total Weight = "<<cost<<endl;
    /*for(int i=0;i<ans.size();i++)
    {
        cout<<ans[i].first<<" "<<ans[i].second<<endl;
    }*/
     return 0;

}
