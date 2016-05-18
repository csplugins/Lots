#include <iostream>
#include <algorithm>
using namespace std;

int main(){
  int n;
  cin >> n;
  int arr[n][n];
  int output[n];
  fill(output, output+n, -1);
  int temp = 1;
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      cin >> arr[i][j];

  while(temp < n+1){
    for(int i = 0; i < n; i++){
      int max = 0;
      for(int j = 0; j < n; j++){
	if(arr[i][j] > max) max = arr[i][j];
      }
      if(max == temp){
	output[i] = temp;
	break;
      }
    }
    temp++;
  }

  for(int i = 0; i < n; i++)
    cout << output[i] << " ";
  cout << endl;
  return 0;
}
