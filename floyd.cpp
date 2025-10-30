#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include <iomanip>
using namespace std;
#define INF 99999

void floydWarshallSerial(vector<vector<int>>& dist, int V) {
    for (int k = 0; k < V; k++)
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
}

void floydWarshallParallel(vector<vector<int>>& dist, int V) {
    for (int k = 0; k < V; k++) {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
}

void printMatrix(vector<vector<int>>& dist, int V) {
    cout << "\nShortest Distance Matrix:\n";
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) cout << setw(7) << "INF";
            else cout << setw(7) << dist[i][j];
        }
        cout << endl;
    }
}

int main() {
    int V = 5;
    vector<vector<int>> graph = {
        {0, 5, INF, 9, INF},
        {INF, 0, 3, INF, INF},
        {INF, INF, 0, 7, INF},
        {11, INF, 4, 0, 3},
        {INF, INF, 14, INF, 0}
    };

    vector<vector<int>> distSerial = graph;
    vector<vector<int>> distParallel = graph;

    double start = omp_get_wtime();
    floydWarshallSerial(distSerial, V);
    double end = omp_get_wtime();
    double serialTime = end - start;

    start = omp_get_wtime();
    floydWarshallParallel(distParallel, V);
    end = omp_get_wtime();
    double parallelTime = end - start;

    cout << "\n--- Floyd-Warshall Serial Version ---";
    printMatrix(distSerial, V);
    cout << "Time (s): " << serialTime << endl;

    cout << "\n--- Floyd-Warshall Parallel Version ---";
    printMatrix(distParallel, V);
    cout << "Time (s): " << parallelTime << endl;

    cout << "\nComparison Table:\n";
    cout << left << setw(20) << "Version" << setw(15) << "Time (s)" << endl;
    cout << "----------------------------------\n";
    cout << setw(20) << "Serial" << setw(15) << serialTime << endl;
    cout << setw(20) << "Parallel" << setw(15) << parallelTime << endl;
}
