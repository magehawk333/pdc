// pi_parallel_reduction.cpp
#include <iostream>
#include <omp.h>
#include <cmath>
using namespace std;

int main() {
    long long num_steps;
    cout << "Enter number of intervals: ";
    cin >> num_steps;

    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 1.0 / (1.0 + x * x);
    }

    double pi = 4.0 * step * sum;

    cout << "Estimated PI (Parallel + reduction) = " << pi << endl;
    cout << "Classical PI (M_PI)                = " << M_PI << endl;
    cout << "Error                             = " << fabs(M_PI - pi) << endl;

    return 0;
}
