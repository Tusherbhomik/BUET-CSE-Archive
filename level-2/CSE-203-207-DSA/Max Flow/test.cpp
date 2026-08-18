#include <bits/stdc++.h>
using namespace std;

const int MAX=10000;
#define N 4

bool bfs(vector<vector<int>>& rGraph, int s, int t, int parent[])
{
	bool visited[N];
	memset(visited, 0, sizeof(visited));
	queue<int> q;
	q.push(s);
	visited[s] = true;
	parent[s] = -1;

	// Standard BFS Loop
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int v = 0; v < N; v++) {
			if (visited[v] == false && rGraph[u][v] > 0) {
				if (v == t) {
					parent[v] = u;
					return true;
				}
				q.push(v);
				parent[v] = u;
				visited[v] = true;
			}
		}
	}
	return false;
}
void fordFulkerson(vector<vector<int>>& graph, int s, int t,int total_game)
{
	int u, v;
    int total_nodes=total_game+N+2;
	vector<vector<int>> rGraph(total_nodes, vector<int>(total_nodes, 0));
	for (u = 0; u < N; u++)
		for (v = 0; v < N; v++)
			rGraph[u][v] = graph[u][v];

	int parent[N]; 
	int max_flow = 0;

	while (bfs(rGraph, s, t, parent)) {
		int path_flow = INT_MAX;
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			path_flow = min(path_flow, rGraph[u][v]);
		}
		for (v = t; v != s; v = parent[v]) {
			u = parent[v];
			rGraph[u][v] -= path_flow;
			rGraph[v][u] += path_flow;
		}
		max_flow += path_flow;
	}
    cout<<"Here is rGraph"<<endl;
    for (int p = 0; p < total_nodes; p++)
        {
            for (int j = 0; j < total_nodes; j++)
            {
                cout << rGraph[p][j] << " ";
            }
            cout << endl;
        }
    for(int i=1;i<=total_game;i++)
    {
        if(rGraph[s][i]>0) 
        {
            cout<<"Eliminated"<<endl;
            break;
        }
    }
	
}
class team
{
public:
    string name;
    int id, wins, losses, left;
    vector<vector<int>> matrix;
    team(string name, int id, int wins, int losses, int left, vector<vector<int>> &matrix)
    {
        this->name = name;
        this->id = id;
        this->wins = wins;
        this->losses = losses;
        this->left = left;
        this->matrix = matrix;
    }
    void show_team_info()
    {
        cout << "team_id:" << id << " " << name << " " << wins << " " << losses << " " << left << endl;
    }
    void show_matrix()
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    
    // freopen("out.txt","w",stdout);
    freopen("in.txt", "r+", stdin);

    int n;
    cin >> n;
    string str;
    int wins, losses, left;
    vector<vector<int>> matrix(n, vector<int>(n, 0));
    vector<team> participants;
    map<string,int> mp;
    for (int i = 0; i < n; i++)
    {
        cin >> str >> wins >> losses >> left;
        for (int j = 0; j < n; j++)
        {
            cin >> matrix[i][j];
        }
        mp[str] = i;
        team t(str, i, wins, losses, left, matrix);
        participants.push_back(t);
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
   
    int total_game = (n - 1) * (n - 2) / 2;
    int total_node=total_game+n+2;
    
    int source = 0;
    int sink = total_node - 1;
    for (int i = 0; i < n; i++)
    {
        vector<vector<int>> graph(total_node, vector<int>(total_node, 0));
        int t = 1;
        for (int j = 0; j < n; j++)
        {
            for (int k = j + 1; k < n; k++)
            {
                if (i == j || i == k)
                    continue;
                graph[source][t] = matrix[j][k];
                graph[t][total_game + i + 1] = MAX;
                graph[t][total_game + k + 1] = MAX;
                t++;
            }
        }

        int w_max;
        bool flag = true;
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                continue;
            w_max = participants[i].wins + participants[i].left - participants[j].wins;
            graph[j+total_game+1][sink] = w_max;
        }
        for (int i = 0; i < total_node; i++)
        {
            for (int j = 0; j < total_node; j++)
            {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
        fordFulkerson(graph,source,sink,total_game); 
    }

    return 0;
}
