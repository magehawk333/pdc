// matrix_mult_openmp.cpp
#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    int N;
    cout << "Enter matrix dimension N (e.g., 3, 100, 1000): ";
    cin >> N;

    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));

    cout << "Enter elements of Matrix A (" << N << "x" << N << "):\n";
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> A[i][j];

    cout << "Enter elements of Matrix B (" << N << "x" << N << "):\n";
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> B[i][j];

    // ---------------- Serial computation ----------------
    double start_serial = omp_get_wtime();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    double end_serial = omp_get_wtime();

    cout << "\nSerial time = " << (end_serial - start_serial) << " seconds\n";

    // ---------------- Parallel computation ----------------
    vector<vector<int>> C_par(N, vector<int>(N, 0));
    double start_parallel = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C_par[i][j] += A[i][k] * B[k][j];

    double end_parallel = omp_get_wtime();
    cout << "Parallel time = " << (end_parallel - start_parallel) << " seconds\n";

    return 0;
}
