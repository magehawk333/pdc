// sum_schedule_static.cpp
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    long long N;
    cout << "Enter N: ";
    cin >> N;
    int chunk;
    cout << "Enter chunk size (for static, C): ";
    cin >> chunk;

    long long sum = 0;

    #pragma omp parallel for schedule(static, chunk) reduction(+:sum)
    for (long long i = 1; i <= N; i++) {
        sum += i;
        cout << "Thread " << omp_get_thread_num() << " processing i=" << i << endl;
    }

    cout << "\nSum (static, chunk=" << chunk << ") = " << sum << endl;
    cout << "Formula (N*(N+1)/2) = " << (N * (N + 1)) / 2 << endl;
    return 0;
}
