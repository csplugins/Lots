#include <iostream>

using namespace std;

int binSearch(int arr[], int start, int end, int search){
  int mid = (end+start)/2;
  if(arr[mid] < search)
    return binSearch(arr, mid+1, end, search);
  else if(arr[mid-1] < search)
    return mid;
  else
    return binSearch(arr, start, mid-1, search);
}

int main(){
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n, m;
  int pair1, pair2;
  cin >> n >> m;
  int arr[++n];
  cin >> pair1 >> pair2;
  arr[0] = 0;
  arr[1] = pair1 * pair2;

  for(int i = 2; i < n; ++i){
    cin >> pair1 >> pair2;
    arr[i] = pair1 * pair2 + arr[i-1];
  }

  for(int i = 0; i < m; ++i){
    cin >> pair1;
    cout << binSearch(arr, 0, n, pair1) << "\n";
  }

}
