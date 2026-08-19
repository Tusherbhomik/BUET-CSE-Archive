#include <bits/stdc++.h>
using namespace std;
int MAX = 10000000;
int V;

int ** Extended_Shortest_path(int **L,int **W)
{
    int**L1 = new int*[V];
    for (int i = 0; i < V; i++) {
        L1[i] = new int[V];
    }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            L1[i][j]=MAX;
            for (int k = 0; k < V; k++)
            {
                L1[i][j] = min(L1[i][j], (L[i][k] + W[k][j]));
            }
        }
    }
    return L1; 
}

void show (int **arr)
{
    for (int i = 0; i < V ; i++)
    {
        for (int j = 0; j < V ; j++)
        {
            if (arr[i][j] >= MAX)
                cout <<setw(5)<< "INF";
            else
                cout <<setw(5)<< arr[i][j];
        }
        cout << endl;
    }
}
int main()
{
    freopen("out.txt", "w", stdout);
    freopen("in.txt", "r+", stdin);
    int v, e, x, y, w;
    cin >> v >> e;
    V=v;
    bool neg_Cycle=false;
    int**arr = new int*[V];
    for (int i = 0; i < V; i++) {
        arr[i] = new int[V];
    }
    int**weight = new int*[V];
    for (int i = 0; i < V; i++) {
        weight[i] = new int[V];
    }
    //initialize
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v ; j++)
        {
            if (i == j)
                weight[i][j] = arr[i][j] = 0;
            else
                weight[i][j] = arr[i][j] = MAX;
        }
    }
    
    //taking edge weight input 
    for (int i = 0; i < e; i++)
    {
        cin >> x >> y >> w;
        weight[x-1][y-1] = arr[x-1][y-1] = w;
    }
    for(int m=1;m<V;m=m*2)
    {
        //check negative cycle 
        for(int i=0;i<V;i++)
        {
            
            if(arr[i][i]!=0)
            {
                neg_Cycle=true;
                break;
            }
        }
        if(neg_Cycle)break;
       // for checking intermediate steps 
       else {
        //show(arr);

        //cout<<'\n'<<endl;
        arr=Extended_Shortest_path(arr,arr);
       }

    }


    if(!neg_Cycle){
    cout<<"Shortest Distance Matrix \n";
    show(arr);
    }
    else 
    {
        cout<<"Negative cycle present"<<endl;
    }
    delete arr,weight;
 
}