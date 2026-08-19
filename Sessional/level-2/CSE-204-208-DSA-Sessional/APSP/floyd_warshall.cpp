#include <bits/stdc++.h>
using namespace std;
int MAX = 1000000;
int main()
{
    freopen("out.txt", "w", stdout);
    freopen("in.txt", "r+", stdin);
    int v, e, x, y, w;
    cin >> v >> e;
    int arr[v][v];
    bool neg_Cycle = false;
    // Initialization
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            if (i == j)
                arr[i][j] = 0;
            else
                arr[i][j] = MAX;
        }
    }
    // taking input
    for (int i = 0; i < e; i++)
    {
        cin >> x >> y >> w;
        arr[x - 1][y - 1] = w;
    }

    for (int k = 0; k < v; k++) // loop for the intermediate vartex
    {
        // check negative cycle
        for (int p = 0; p < v; p++)
        {
            if (arr[p][p] != 0)
            {
                neg_Cycle = true;
                break;
            }
        }
        if (neg_Cycle)
            break;
        else
        {
            for (int i = 0; i < v; i++)
            {
                for (int j = 0; j < v; j++)
                {
                    //if (i == j || i == k && j == k)
                     //   continue;
                    arr[i][j] = min(arr[i][j], arr[i][k] + arr[k][j]);
                }
            }
        }
        // for intermediate steps
        /*for (int i = 0; i < v ; i++)
         {
             for (int j = 0; j < v; j++)
             {
                 if (arr[i][j] >= MAX)
                     cout<<setw(5) << "INF";
                 else
                     cout <<setw(5)<< arr[i][j];
             }
             cout << endl;
         }
         cout<<endl;*/
    }
    // Print
    if (!neg_Cycle)
    {
        cout << "Shortest Distance Matrix \n";
        for (int i = 0; i < v; i++)
        {
            for (int j = 0; j < v; j++)
            {
                if (arr[i][j] >= MAX)
                    cout << setw(5) << "INF";
                else
                    cout << setw(5) << arr[i][j];
            }
            cout << endl;
        }
    }
    else
    {
        cout << "Negative weight cycle present" << endl;
    }
    return 0;
}