// gaussian_parallel.cpp
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void print_vec(const vector<double>& v) {
    for (double x : v) cout << x << " ";
    cout << "\n";
}

int main(int argc, char** argv) {
    // Optionally set number of threads via environment or here:
    // omp_set_num_threads(4);

    int N = 3;
    vector<vector<double>> A = {
        {1, -1, 1},
        {1, -4, 2},
        {1, 2, 8}
    };
    vector<double> b = {4, 8, 12};

    double t0 = omp_get_wtime();

    // We will record which thread updates which row for visibility
    vector<int> rowUpdatedBy(N, -1);

    for (int k = 0; k < N-1; ++k) {
        int pivot = k;
        double maxv = fabs(A[k][k]);

        // pivot selection must be serial (protect shared vars). Use single.
        #pragma omp parallel
        {
            #pragma omp single
            {
                int num_threads = omp_get_num_threads();
                // find pivot in single thread
                for (int i = k+1; i < N; ++i) {
                    double v = fabs(A[i][k]);
                    if (v > maxv) { maxv = v; pivot = i; }
                }
                if (pivot != k) {
                    swap(A[k], A[pivot]);
                    swap(b[k], b[pivot]);
                }
            } // end single
            #pragma omp barrier
            // Now parallelize elimination of rows i = k+1 .. N-1
            #pragma omp for schedule(static)
            for (int i = k+1; i < N; ++i) {
                int tid = omp_get_thread_num();
                rowUpdatedBy[i] = tid; // mark which thread handles this row
                double factor = A[i][k] / A[k][k];
                A[i][k] = 0.0;
                for (int j = k+1; j < N; ++j) {
                    A[i][j] -= factor * A[k][j];
                }
                b[i] -= factor * b[k];
                // (no race here because each thread writes different rows i)
            }
            #pragma omp barrier
        } // end parallel region for this k
    } // end for k

    // Back substitution (serial for correctness and simplicity)
    vector<double> x(N);
    for (int i = N-1; i >= 0; --i) {
        double s = b[i];
        for (int j = i+1; j < N; ++j) s -= A[i][j] * x[j];
        x[i] = s / A[i][i];
    }

    double t1 = omp_get_wtime();

    // Print which threads updated which rows
    cout << "Parallel elimination row assignments (row -> thread):\n";
    for (int i = 0; i < N; ++i) {
        cout << "row " << i << " updated by thread " << rowUpdatedBy[i] << "\n";
    }

    cout.setf(std::ios::fixed); cout<<setprecision(7);
    cout << "Parallel solution x: ";
    print_vec(x);
    cout << "Expected solution approx: 1.6666667  -0.8333333  1.5000000\n";
    cout << "Parallel execution time (seconds): " << (t1 - t0) << "\n";
    cout << "Number of threads used (runtime): " << omp_get_max_threads() << "\n";
    return 0;
}
