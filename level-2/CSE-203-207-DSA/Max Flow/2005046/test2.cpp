#include <bits/stdc++.h>
using namespace std;
int V;
class team
{
public:
    string name;
    int id, wins, losses, left;
    team(string name, int id, int wins, int losses, int left)
    {
        this->name = name;
        this->id = id;
        this->wins = wins;
        this->losses = losses;
        this->left = left;
    }
    void show_team_info()
    {
        cout << "team_id:" << id << " " << name << " " << wins << " " << losses << " " << left << endl;
    }
};

bool bfs(vector<vector<int>> &rGraph, int s, int t, vector<int> &parent)
{
    int n = rGraph.size();
    vector<bool> visited(n, false);

    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v = 0; v < n; v++)
        {
            if (!visited[v] && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return (visited[t] == true);
}

void edmonds_Karp(vector<vector<int>> &graph, int s, int t, int total_game, vector<team> &participants, int id)
{
    int u, v;
    int t_node = total_game + V + 2;
    vector<vector<int>> newGraph(t_node, vector<int>(t_node, 0));
    for (u = 0; u < t_node; u++)
    {
        for (v = 0; v < t_node; v++)
        {
            newGraph[u][v] = graph[u][v];
        }
    }
    vector<int> parent(V, -1);
    int max_flow = 0;
    int p_flow = INT_MAX;
    bool end;
    end=bfs(newGraph, s, t, parent);
    while (end)
    {

        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            p_flow=(p_flow<newGraph[u][v])?p_flow:newGraph[u][v];
        }

        for (v = t; v != s; v = parent[v])
        {
             u = parent[v];
            newGraph[u][v] = newGraph[u][v] - p_flow;
            newGraph[v][u] = p_flow +newGraph[v][u];
        }

        max_flow += p_flow;
        end=bfs(newGraph, s, t, parent);
    }
    for (int h = 1; h <= total_game; h++)
    {
        if (newGraph[s][h] > 0)
        {
            cout << participants[id].name << " is eliminated" << endl;
            break;
        }
    }
}

int main()
{
    freopen("out.txt", "w", stdout);
    freopen("in.txt", "r+", stdin);
    int n;
    cin >> n;
    V = n;
    string str;
    int wins, losses, left;
    vector<vector<int>> matrix(V, vector<int>(V, 0));
    vector<team> participants;
    for (int i = 0; i < V; i++)
    {
        cin >> str >> wins >> losses >> left;
        for (int j = 0; j < V; j++)
        {
            cin >> matrix[i][j];
        }
        team t(str, i, wins, losses, left);
        participants.push_back(t);
    }

    int total_game = (V - 1) * (V - 2) / 2;
    int total_node = total_game + V + 2;

    int source = 0;
    int sink = total_node - 1;
    for (int i = 0; i < V; i++)
    {
        vector<vector<int>> graph(total_node, vector<int>(total_node, 0));
        int t = 1;
        for (int j = 0; j < V; j++)
        {
            for (int k = j + 1; k < V; k++)
            {
                if (i == j || i == k)
                    continue;
                 graph[source][t] = matrix[j][k];
                 graph[t][total_game + i + 1] = INT_MAX;
                 graph[t][total_game + k + 1] = INT_MAX;
                 t++;
            }
        }

        int w_max;
        for (int j = 0; j < V; j++)
        {
            if (i == j)
                continue;
            w_max = participants[i].wins + participants[i].left - participants[j].wins;
            graph[j + total_game + 1][sink] = w_max;
        }

        edmonds_Karp(graph, source, sink, total_game, participants, i);
    }
}
