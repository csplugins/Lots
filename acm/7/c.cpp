#include <iostream>

using namespace std;

int main(){
  int n, total = 0;
  cin >> n;
  int arr[2*n-1];
  for(int i = 0; i < 2*n-1; ++i){
    int temp;
    cin >> temp;
    arr[i] = temp;
    total += temp;
  }
}
