#include<bits/stdc++.h>
using namespace std;

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
vector<pair<int, int>> kruskalMST(vector<vector<int>>&adjMat)
{
    vector<pair<int, int>> pairVector;
    int mincost = 0; // Cost of min MST.
    int parent[adjMat.size()];
    for (int i = 0; i < adjMat.size(); i++)
        parent[i] = i;

    int edge_count = 0;
    while (edge_count < adjMat.size() - 1) {
        int min = 100000, a = -1, b = -1;
        for (int i = 0; i < adjMat.size(); i++) {
            for (int j = 0; j < adjMat.size(); j++) {
                if (find_set(i,parent) != find_set(j,parent) && adjMat[i][j] < min) {
                    min = adjMat[i][j];
                    a = i;
                    b = j;
                }
            }
        }
        union_sets(a, b,parent);
        edge_count++;
        pairVector.push_back({a,b});
        // printf("Edge %d:(%d, %d) cost:%d \n",
        //        edge_count++, a, b, min);
        mincost += min;
    }
    // printf("\n Minimum cost= %d \n", mincost);
    // for (int i = 0; i < pairVector.size(); i++)
    //     printf("%d - %d\n", pairVector[i].first, pairVector[i].second);
    return pairVector;
}
// int main()
// {
// 	vector<vector<int>> distVector = {
//     {0, 88, 68, 148, 71, 147, 69},
//     {88, 0, 69, 197, 50, 141, 71},
//     {68, 69, 0, 196, 72, 116, 83},
//     {148, 197, 196, 0, 192, 192, 161},
//     {71, 50, 72, 192, 0, 148, 114},
//     {147, 141, 116, 192, 148, 0, 192},
//     {69, 71, 83, 161, 114, 192, 0}};


// 	kruskalMST(distVector);

// 	return 0;
// }





