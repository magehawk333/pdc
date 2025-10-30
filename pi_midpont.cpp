// pi_rectangle_rule.cpp
#include <iostream>
#include <cmath>
#include <omp.h>
using namespace std;

// Function to integrate
inline double f(double x) {
    return 4.0 / (1.0 + x * x);
}

int main() {
    int n_values[] = {10, 50, 100, 500, 1000};
    int num_cases = 5;
    double a = 0.0, b = 1.0;
    double pi_true = M_PI;

    cout << "----------------------------------------------\n";
    cout << "Estimation of PI using Rectangle Rule (f(x)=4/(1+x^2))\n";
    cout << "a=0, b=1, n={10,50,100,500,1000}\n";
    cout << "----------------------------------------------\n\n";

    for (int k = 0; k < num_cases; k++) {
        int n = n_values[k];
        double dx = (b - a) / n;
        double left_sum = 0.0, right_sum = 0.0, mid_sum = 0.0;

        double start_time = omp_get_wtime();

        // ----- Case 1: Left Rectangle -----
        for (int i = 0; i < n; i++) {
            double x = a + i * dx;
            left_sum += f(x);
        }
        double pi_left = dx * left_sum;

        // ----- Case 2: Right Rectangle -----
        for (int i = 0; i < n; i++) {
            double x = a + (i + 1) * dx;
            right_sum += f(x);
        }
        double pi_right = dx * right_sum;

        // ----- Case 3: Midpoint Rectangle -----
        for (int i = 0; i < n; i++) {
            double x = a + (i + 0.5) * dx;
            mid_sum += f(x);
        }
        double pi_mid = dx * mid_sum;

        double end_time = omp_get_wtime();

        cout << "n = " << n << ":\n";
        cout << "  Left  PI  = " << pi_left << "   Error = " << fabs(pi_true - pi_left) << endl;
        cout << "  Right PI  = " << pi_right << "   Error = " << fabs(pi_true - pi_right) << endl;
        cout << "  Mid   PI  = " << pi_mid << "   Error = " << fabs(pi_true - pi_mid) << endl;
        cout << "  Execution Time (Serial) = " << (end_time - start_time) << " sec\n";
        cout << "----------------------------------------------\n";
    }

    cout << "\n=========== PARALLEL VERSION ===========\n";

    int n = 10000000;  // Larger n to notice speedup
    double dx = (b - a) / n;
    double sum = 0.0;

    // ---------- Parallel (with Race Condition) ----------
    double start_time = omp_get_wtime();
    #pragma omp parallel
    {
        double local_sum = 0.0;
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();

        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = a + (i + 0.5) * dx; // midpoint
            local_sum += f(x);
        }

        // Race condition occurs here without synchronization
        sum += local_sum;

        cout << "Thread " << tid << " finished partial area = " << dx * local_sum << endl;
    }
    double pi_parallel_wrong = dx * sum;
    double end_time = omp_get_wtime();

    cout << "\n[Parallel without protection] PI = " << pi_parallel_wrong
         << "   Error = " << fabs(M_PI - pi_parallel_wrong)
         << "   Time = " << (end_time - start_time) << " sec" << endl;

    // ---------- Fix using 'critical' ----------
    sum = 0.0;
    start_time = omp_get_wtime();
    #pragma omp parallel
    {
        double local_sum = 0.0;
        #pragma omp for
        for (int i = 0; i < n; i++) {
            double x = a + (i + 0.5) * dx;
            local_sum += f(x);
        }
        #pragma omp critical
        sum += local_sum;
    }
    double pi_critical = dx * sum;
    end_time = omp_get_wtime();
    cout << "\n[Parallel with critical] PI = " << pi_critical
         << "   Error = " << fabs(M_PI - pi_critical)
         << "   Time = " << (end_time - start_time) << " sec" << endl;

    // ---------- Fix using 'atomic' ----------
    sum = 0.0;
    start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * dx;
        double val = f(x);
        #pragma omp atomic
        sum += val;
    }
    double pi_atomic = dx * sum;
    end_time = omp_get_wtime();
    cout << "[Parallel with atomic]  PI = " << pi_atomic
         << "   Error = " << fabs(M_PI - pi_atomic)
         << "   Time = " << (end_time - start_time) << " sec" << endl;

    // ---------- Fix using 'reduction' ----------
    sum = 0.0;
    start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * dx;
        sum += f(x);
    }
    double pi_reduction = dx * sum;
    end_time = omp_get_wtime();
    cout << "[Parallel with reduction] PI = " << pi_reduction
         << "   Error = " << fabs(M_PI - pi_reduction)
         << "   Time = " << (end_time - start_time) << " sec" << endl;

    cout << "----------------------------------------------\n";
    cout << "Classical PI = " << M_PI << endl;
    cout << "----------------------------------------------\n";
    return 0;
}
