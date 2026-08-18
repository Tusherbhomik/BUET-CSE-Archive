#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>

using namespace std;
int randomNumber(int minWeight, int maxWeight, std::mt19937& random) {
    
    std::uniform_int_distribution<int> distribution(minWeight, maxWeight + 1);
    return distribution(random);
}

vector<vector<int>> generateCompleteGraph(int n, int minWeight, int maxWeight, std::mt19937& random) {
    vector<vector<int>> adjacencyMatrix(n, vector<int>(n, 0));
    cout<<"minWeight: "<<minWeight<<endl;
    cout<<"maxWeight: "<<maxWeight<<endl;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            
            int weight = randomNumber(minWeight, maxWeight, random);
            cout<<"weight: "<<weight<<endl;
            // Check and enforce the triangular inequality
            for (int k = 0; k < n; k++) {
                if (k != i && k != j) {
                    int weightIK = adjacencyMatrix[i][k];
                    int weightKJ = adjacencyMatrix[k][j];
                    if (weightIK + weightKJ < weight) {
                        // Adjust the weight of the edge (i, j) to satisfy the triangular inequality
                        weight = weightIK + weightKJ;
                        cout<<"weight: "<<weight<<endl;
                    }
                }
            }

            
            adjacencyMatrix[i][j] = weight;
            adjacencyMatrix[j][i] = weight; 
        }
    }

    return adjacencyMatrix;
}

int main() {
    std::random_device generator;
    std::mt19937 random(generator());
    int n = 4;
    int minWeight = 10;
    int maxWeight = 100;

    // Generate the complete graph
    vector<vector<int>> adjacencyMatrix = generateCompleteGraph(n, minWeight, maxWeight, random);

    // Print the adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
