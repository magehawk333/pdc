#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include <iomanip>
using namespace std;
#define INF INT_MAX

int minDistance(vector<int>& dist, vector<bool>& visited, int V) {
    int minVal = INF, minIndex = -1;

    #pragma omp parallel for
    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] < minVal) {
            #pragma omp critical
            {
                if (dist[v] < minVal) {
                    minVal = dist[v];
                    minIndex = v;
                }
            }
        }
    }
    return minIndex;
}

void dijkstraSerial(vector<vector<int>>& graph, int src, int V, vector<int>& dist) {
    vector<bool> visited(V, false);
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = -1, minVal = INF;
        for (int v = 0; v < V; v++)
            if (!visited[v] && dist[v] <= minVal)
                minVal = dist[v], u = v;

        visited[u] = true;
        for (int v = 0; v < V; v++)
            if (!visited[v] && graph[u][v] && dist[u] != INF
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
}

void dijkstraParallel(vector<vector<int>>& graph, int src, int V, vector<int>& dist) {
    vector<bool> visited(V, false);
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited, V);
        visited[u] = true;

        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INF) {
                int newDist = dist[u] + graph[u][v];
                if (newDist < dist[v]) {
                    #pragma omp critical
                    dist[v] = newDist;
                }
            }
        }
    }
}

void printResult(vector<int>& dist, int V) {
    cout << "\nVertex\tDistance from Source\n";
    for (int i = 0; i < V; i++)
        cout << "  " << i << "\t\t" << dist[i] << endl;
}

int main() {
    int V = 6;
    vector<vector<int>> graph = {
        {0,7,12,0,0,0},
        {0,0,2,9,0,0},
        {0,0,0,0,10,0},
        {0,0,0,0,0,1},
        {0,0,0,4,0,5},
        {0,0,0,0,0,0}
    };

    vector<int> distSerial(V, INF), distParallel(V, INF);

    double start = omp_get_wtime();
    dijkstraSerial(graph, 0, V, distSerial);
    double end = omp_get_wtime();
    double serialTime = end - start;

    start = omp_get_wtime();
    dijkstraParallel(graph, 0, V, distParallel);
    end = omp_get_wtime();
    double parallelTime = end - start;

    cout << "\n--- Dijkstra Serial Version ---";
    printResult(distSerial, V);
    cout << "Time (s): " << serialTime << endl;

    cout << "\n--- Dijkstra Parallel Version ---";
    printResult(distParallel, V);
    cout << "Time (s): " << parallelTime << endl;

    cout << "\nComparison Table:\n";
    cout << left << setw(20) << "Version" << setw(15) << "Time (s)" << endl;
    cout << "----------------------------------\n";
    cout << setw(20) << "Serial" << setw(15) << serialTime << endl;
    cout << setw(20) << "Parallel" << setw(15) << parallelTime << endl;
}
