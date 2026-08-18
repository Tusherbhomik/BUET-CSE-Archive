#include <bits/stdc++.h>
using namespace std;
map<string, int> mp;
int N;
const int MAX = 10000;
// bool found_augmented_path(vector<vector<int>> &graph, int source, int sink, vector<bool> visited)
// {

//     queue<int> q;
//     q.push(source);
//     visited[source] = true;
//     while (!q.empty())
//     {
//         int front = q.front();
//         q.pop();
//         for (int i = 0; i < N; i++)
//         {
//             if (graph[source][i] != 0 && visited[i] == false)
//             {
//                 if (i == sink)
//                 {
//                     return true;
//                 }
//                 q.push(i);
//                 visited[i] = true;
//             }
//         }
//     }
//     return false;
// }

// void edmonkarp(vector<vector<int>> &graph, int source, int sink)
// {
//     vector<bool> visited;
//     for (int i = 0; i < N; i++)
//     {
//         visited[i] = false;
//     }
//     while (found_augmented_path(graph, source, sink, visited))
//     {
//         cout << "Hello" << endl;
//         int parent[N];
//         queue<int> q;
//         q.push(source);
//         visited[source] = true;

//         while (!q.empty())
//         {
//             int front = q.front();
//             q.pop();
//             if (front == sink)
//             {
//                 int p = sink;
//                 int flow = 0;
//                 flow = min(flow, graph[parent[sink]][sink]);
//                 flow = min(flow, graph[source][parent[parent[sink]]]);
//                 graph[parent[sink]][sink] -= flow;
//                 graph[parent[parent[sink]]][parent[sink]] -= flow;
//                 graph[source][parent[parent[sink]]] -= flow;
//             }
//             for (int i = 0; i < N; i++)
//             {
//                 if (graph[source][i] != 0 && visited[i] == false)
//                 {
//                     q.push(i);
//                     parent[i] = front;
//                     visited[i] = true;
//                 }
//             }
//         }
//     }
// }

bool bfs(vector<vector<int>> &rGraph, int s, int t, int parent[])
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

void fordFulkerson(vector<vector<int>> &graph, int s, int t,int total_game)
{
    
	int u, v;
	vector<vector<int>> rGraph(total_game+N+2, vector<int>(total_game+N+2, 0));
    
	for (u = 0; u < N; u++)
		for (v = 0; v < N; v++)
			rGraph[u][v]=graph[u][v];

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
    cout<<"Residue"<<endl;
    for(int p=0;p<rGraph.size();p++)
        {
            for(int q=0;q<rGraph.size();q++)
            {
                cout<<rGraph[p][q]<<" ";
            }
            cout<<endl;
        }
        for(int h=1;h<=total_game;h++)
        {
            if(rGraph[s][h]>0)
            { cout<<":eliminated"<<endl;
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
    N = n;
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

    for (int i = 0; i < participants.size(); i++)
    {
        participants[i].show_team_info();
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
        cout<<"Here for :"<<i<<endl;
        for (int i = 0; i < total_node; i++)
        {
            for (int j = 0; j < total_node; j++)
            {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
        //fordFulkerson(graph,source,sink,total_game);
        
    }

    return 0;
}