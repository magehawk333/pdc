// quicksort_openmp.cpp
#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Partition function
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Pivot = last element
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Parallel QuickSort
void quicksort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp task shared(arr)
        quicksort(arr, low, pi - 1);

        #pragma omp task shared(arr)
        quicksort(arr, pi + 1, high);
    }
}

// Wrapper to start parallel region
void quicksort_parallel(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            quicksort(arr, 0, arr.size() - 1);
        }
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];

    double start_time = omp_get_wtime();
    quicksort_parallel(arr);
    double end_time = omp_get_wtime();

    cout << "\nSorted array:\n";
    for (int x : arr) cout << x << " ";
    cout << "\nExecution Time: " << (end_time - start_time) << " seconds\n";

    cout << "\nTime Complexities:\n";
    cout << "Best Case: O(N log N)\n";
    cout << "Average Case: O(N log N)\n";
    cout << "Worst Case: O(N^2)\n";

    return 0;
}
