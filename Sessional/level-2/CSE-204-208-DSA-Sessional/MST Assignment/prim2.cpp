#include<bits/stdc++.h>
#include<typeinfo>
using namespace std;

bool compair(const pair< pair<int,int>,int> &a, const pair< pair<int,int>,int> &b)
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

int main()
{
    freopen("in.txt","r",stdin);
    freopen("prim.txt","w",stdout);
    int n,m;
    cin>>n>>m;
    int root=0;
    vector<pair<int,double>>adj[n];//vector of pair <destination,weight>
    vector<pair<int,int>>ans;
    int u,v,d;
    for(int i=0; i<m; i++)
    {
        cin>>u>>v>>d;
        adj[u].push_back(make_pair(v,d));
        adj[v].push_back(make_pair(u,d));
    }
    bool visited[n];
    memset(visited,false,sizeof(visited));


    priority_queue< pair< pair<int,int>,double>,vector<pair< pair<int,int>,double>>,decltype(&compair)> pq(compair);
    //start from zero

    for(pair<int,int> u:adj[root])
    {
        pq.push(make_pair(make_pair(root,u.first),u.second));
    }
    visited[root]=true;
    double cost=root;

    while(!pq.empty())
    {
        int des=pq.top().first.second;
        if (visited[des])
        {
            pq.pop();
        }
        else
        {
            visited[des]=true;
            cost+=pq.top().second;
            ans.push_back(make_pair(pq.top().first.first,pq.top().first.second));
            //cout<<pq.top().first.first<<"  "<<pq.top().first.second<<endl;
            pq.pop();
            for(pair<int,double> u:adj[des])
            {
                pq.push(make_pair(make_pair(des,u.first),u.second));
            }
        }

    }
    cout<<"Total Weight = "<<cost<<endl;
    cout<<"Root node = "<<0<<endl;
    for(int i=0;i<ans.size();i++)
    {
        cout<<ans[i].first<<" "<<ans[i].second<<endl;
    }
    return 0;

}
