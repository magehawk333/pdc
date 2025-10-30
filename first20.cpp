#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main(){
    int sum=0;
    int n=20;
    vector<int> nums(n);
  #pragma omp parallel for reduction(+:sum)
    for (int i=0;i<n;i++){
        nums[i]=i+1;
        sum+=nums[i];
    }
    cout <<"solutions:\n";
    for (int x:nums)cout << x << " ";
    cout <<"\n SUm" << sum <<endl;
    return 0;

}