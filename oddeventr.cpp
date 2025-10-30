// parallel_oddeven_sort.cpp
#include <iostream>
#include <omp.h>
using namespace std;

void parallelOddEvenSort(int arr[], int n) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;

        // ----- Odd Phase -----
        #pragma omp parallel for shared(arr, sorted)
        for (int i = 1; i <= n - 2; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }

        // ----- Even Phase -----
        #pragma omp parallel for shared(arr, sorted)
        for (int i = 0; i <= n - 2; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                sorted = false;
            }
        }
    }
}

int main() {
    int arr[] = {10, 55, 9, 4, 2, 7, 3, 5, 6, 1};
    int n = 10;
    int threads = 4;

    omp_set_num_threads(threads);

    double start = omp_get_wtime();
    parallelOddEvenSort(arr, n);
    double end = omp_get_wtime();

    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\nThreads Used: " << threads
         << "\nExecution Time: " << (end - start) << " sec\n";

    cout << "Asymptotic Time Complexity: O(N^2 / P)\n";
    return 0;
}
