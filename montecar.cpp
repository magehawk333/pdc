// parallel_montecarlo_pi_fixed.cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>
using namespace std;

int main() {
    long long N = 10000000;
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    // ----- Using critical -----
    long long count = 0;
    double start = omp_get_wtime();
    #pragma omp parallel
    {
        unsigned int seed = time(0) + omp_get_thread_num();
        long long local_count = 0;

        #pragma omp for
        for (long long i = 0; i < N; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0)
                local_count++;
        }

        #pragma omp critical
        count += local_count;
    }
    double pi_critical = 4.0 * count / N;
    double end = omp_get_wtime();
    cout << "[Critical] Pi = " << pi_critical
         << "  Time = " << (end - start) << " sec\n";

    // ----- Using atomic -----
    count = 0;
    start = omp_get_wtime();
    #pragma omp parallel
    {
        unsigned int seed = time(0) + omp_get_thread_num();
        #pragma omp for
        for (long long i = 0; i < N; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x * x + y * y <= 1.0)
                #pragma omp atomic
                count++;
        }
    }
    double pi_atomic = 4.0 * count / N;
    end = omp_get_wtime();
    cout << "[Atomic]   Pi = " << pi_atomic
         << "  Time = " << (end - start) << " sec\n";

    // ----- Using reduction -----
    count = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:count)
    for (long long i = 0; i < N; i++) {
        unsigned int seed = time(0) + i;
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0)
            count++;
    }
    double pi_reduction = 4.0 * count / N;
    end = omp_get_wtime();
    cout << "[Reduction] Pi = " << pi_reduction
         << "  Time = " << (end - start) << " sec\n";

    cout << "True Pi = " << M_PI << endl;
    return 0;
}
