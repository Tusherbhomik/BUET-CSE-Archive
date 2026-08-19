#include <bits/stdc++.h>
#include "Kruskal.cpp"
using namespace std;

const int minWeight = 50;
const int maxWeight = 200;
const int INF = 100000;
int generate_random_weight()
{
    return rand() % (maxWeight - minWeight + 1) + minWeight;
}
vector<vector<int>> Create_Random_Graph(int num_vertices)
{
    srand(time(0));
    vector<vector<int>> adj_matrix(num_vertices, vector<int>(num_vertices, 0));

    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = i + 1; j < num_vertices; j++)
        {
            int weight = generate_random_weight();
            adj_matrix[i][j] = weight;
            adj_matrix[j][i] = weight;
        }
    }
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = i + 1; j < num_vertices; j++)
        {
            for (int k = 0; k < num_vertices; k++)
            {
                if (k != i && k != j)
                {
                    int min_weight = min(adj_matrix[i][k], adj_matrix[j][k]);
                    int max_weight = max(adj_matrix[i][j], min_weight + 1); // Ensure direct edges have higher weights.
                    adj_matrix[i][j] = max_weight;
                    adj_matrix[j][i] = max_weight;
                }
            }
        }
    }

    for (int i = 0; i < num_vertices; i++)
    {
        adj_matrix[i][i] = 0;
    }
    return adj_matrix;
}

void print(vector<vector<int>> &adjacencyMatrix)
{
    int n = adjacencyMatrix.size();
    cout << "Random Edge Weights (satisfying triangular inequality):" << endl;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << adjacencyMatrix[i][j] << "\t";
        }
        cout << endl;
    }
}

class Graph
{
    int n;
    vector<vector<int>> adjacencyMatrix;

public:
    Graph()
    {
        n = 0;
    }
    void setGraph(vector<vector<int>> graph)
    {
        adjacencyMatrix = graph;
        n = graph.size();
    }
    vector<vector<int>> &getGraph()
    {
        return adjacencyMatrix;
    }
    int getVertices()
    {
        return n;
    }
    int getWeight(int u, int v)
    {
        return adjacencyMatrix[u][v];
    }
};
int Exact_TSP_Utill(int mask, int pos, Graph &G, vector<vector<int>> &dp)
{
        for (int mask = (1 << G.getVertices()) - 1; mask >= 0; mask--){
            for (int pos = 0; pos < G.getVertices(); pos++){
                if (mask == (1 << G.getVertices()) - 1)
                    dp[mask][pos] = G.getWeight(pos, 0);
                else
                {
                    int ans = INF;
                    for (int vertex = 0; vertex < G.getVertices(); vertex++)
                        if ((mask & (1 << vertex)) == 0)
                        {
                            int temp = G.getWeight(pos, vertex) + dp[mask | (1 << vertex)][vertex];
                            if (temp < ans)
                            {
                                ans = temp;
                            }
                        }
                    dp[mask][pos] = ans;
                }
            }
    }
    

    return dp[mask][pos];
}

int Exact_TSP(Graph &G)
{
    vector<vector<int>> dp(1 << G.getVertices(), vector<int>(G.getVertices(), -1));
    int ans = Exact_TSP_Utill(1, 0, G, dp);
    return ans;
}

vector<int>  Metric_Approximation_TSP(Graph &G)
{
    vector<pair<int, int>> pairVector = kruskalMST(G.getGraph());
    int n = G.getVertices();
    vector<pair<int, int>> adjacencyList[n];
    for (int  i = 0; i < pairVector.size(); i++)
    {
        adjacencyList[pairVector[i].first].push_back({pairVector[i].second, G.getWeight(pairVector[i].first, pairVector[i].second)});
        adjacencyList[pairVector[i].second].push_back({pairVector[i].first, G.getWeight(pairVector[i].first, pairVector[i].second)});
    }

    vector<int> path;
    vector<bool> visited(n, false);
    stack<int> st;
    st.push(0);
    visited[0] = true;
    while (!st.empty())
    {
        int u = st.top();
        st.pop();
        path.push_back(u);
        for (int i = 0; i < adjacencyList[u].size(); i++)
        {
            int v = adjacencyList[u][i].first;
            if (!visited[v])
            {
                visited[v] = true;
                st.push(v);
            }
        }
    }
    path.push_back(0);
    return path;
}
vector<vector<int>> distVector = {
    {0, 88, 68, 148, 71, 147, 69},
    {88, 0, 69, 197, 50, 141, 71},
    {68, 69, 0, 196, 72, 116, 83},
    {148, 197, 196, 0, 192, 192, 161},
    {71, 50, 72, 192, 0, 148, 114},
    {147, 141, 116, 192, 148, 0, 192},
    {69, 71, 83, 161, 114, 192, 0}};
int Calculate_Tour_Length(vector<int> Tour,Graph &G){
    int cost = 0;
    for (int i = 0; i < Tour.size() - 1; i++)
    {
        cost += G.getWeight(Tour[i], Tour[i + 1]);
    }
    return cost;
}
int main()
{
    int X,i=0;
    cin >>X;
    
    vector<double> exactTspCosts;
    vector<double> approxTspCosts;
    while(i<X){
        vector<vector<int>> adj_matrix = Create_Random_Graph(20);
        Graph g;
        g.setGraph(adj_matrix);
        // print(g.getGraph());
        exactTspCosts.push_back(Exact_TSP(g));
        vector<int>matAppTour= Metric_Approximation_TSP(g); 
        approxTspCosts.push_back(Calculate_Tour_Length(matAppTour,g));
        i++;
        adj_matrix.clear();
        g.getGraph().clear();
        

    }
    for (int i = 0; i < exactTspCosts.size(); i++)
    {
        cout<<approxTspCosts[i]<<endl;
        cout<<exactTspCosts[i]<<endl;
        cout<<"Pair ("<<i+1<<","<<approxTspCosts[i]/exactTspCosts[i]<<")"<<endl;
    }

    return 0;
}
