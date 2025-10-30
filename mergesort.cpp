#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

// Merge two sorted subarrays arr[l..m] and arr[m+1..r]
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

// Recursive Merge Sort with OpenMP parallel tasks
void parallelMergeSort(vector<int>& arr, int l, int r, int depth = 0) {
    if (l >= r) return;

    int m = (l + r) / 2;

    if (r - l < 1000) {
        // Small problem → sort serially to avoid overhead
        parallelMergeSort(arr, l, m);
        parallelMergeSort(arr, m + 1, r);
    } else {
        #pragma omp task shared(arr)
        parallelMergeSort(arr, l, m);

        #pragma omp task shared(arr)
        parallelMergeSort(arr, m + 1, r);

        #pragma omp taskwait
    }

    merge(arr, l, m, r);
}

// Wrapper to start parallel region
void mergeSortParallel(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelMergeSort(arr, 0, arr.size() - 1);
        }
    }
}

// Serial MergeSort for comparison
void serialMergeSort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    serialMergeSort(arr, l, m);
    serialMergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Main function
int main() {
    int N = 1000000;  // array size
    vector<int> arr(N), arrCopy(N);

    // Generate random array
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 1000000;
    arrCopy = arr;

    double start, end;

    // ---- Serial Execution ----
    start = omp_get_wtime();
    serialMergeSort(arr, 0, N - 1);
    end = omp_get_wtime();
    double serialTime = end - start;

    // ---- Parallel Execution ----
    start = omp_get_wtime();
    mergeSortParallel(arrCopy);
    end = omp_get_wtime();
    double parallelTime = end - start;

    cout << fixed << setprecision(6);
    cout << "Array Size: " << N << endl;
    cout << setw(25) << left << "Serial MergeSort Time: " << serialTime << " sec\n";
    cout << setw(25) << left << "Parallel MergeSort Time: " << parallelTime << " sec\n";
    cout << setw(25) << left << "Speedup: " << serialTime / parallelTime << "x\n";

    return 0;
}
