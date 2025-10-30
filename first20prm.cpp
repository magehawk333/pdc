#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main() {
    vector<int> primes;
    int num = 2, sum = 0;

    #pragma omp parallel
    {
        vector<int> local_primes;
        #pragma omp for nowait
        for (int i = 2; i < 1000; i++) {
            if (isPrime(i))
                local_primes.push_back(i);
        }
        #pragma omp critical
        primes.insert(primes.end(), local_primes.begin(), local_primes.end());
    }

    sort(primes.begin(), primes.end());
    primes.resize(20);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 20; i++) sum += primes[i];

    cout << "First 20 Prime Numbers:\n";
    for (int x : primes) cout << x << " ";
    cout << "\nSum = " << sum << endl;
    return 0;
}
