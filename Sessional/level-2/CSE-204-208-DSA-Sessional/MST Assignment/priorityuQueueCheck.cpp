#include<iostream>
#include<vector>
#include<queue>
using namespace std;
void addEdge(vector<pair<int, int>> adj[], int u, int v, int d)
{
    adj[u].push_back(make_pair(v, d));
    adj[v].push_back(make_pair(u, d));
}
bool compair(const pair<int, pair<int, int>> &a, const pair<int, pair<int, int>> &b)
{
    if (a.second.second > b.second.second)
    {
        return true;
    }
    else if (a.second.second == b.second.second)
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
    int n, m;
    cin >> n >> m;
    
    
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, decltype(&compair)> pq(compair);
    pq.push(make_pair(2,make_pair(5,7)));
    pq.push(make_pair(2,make_pair(5,5)));
    pq.push(make_pair(2,make_pair(5,8)));
    pq.push(make_pair(2,make_pair(5,4)));
    pq.push(make_pair(2,make_pair(5,2)));
    while(!pq.empty())
    {
        cout<<pq.top().second.second;
        pq.pop();
    }
    // start from node --->0

    return 0;
}