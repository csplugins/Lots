#include <iostream>
using namespace std;

int check[100001];

int main(){
  int n;
  int pos = 0;
  string dir;
  cin >> n >> dir;
  int arr[n];
  check[0] = 1;
  for(int i = 0; i < n; ++i){
    cin >> arr[i];
  }
  for(int i = 0; i < n * 3; ++i){
    if(dir[pos] == '>')
      pos += arr[pos];
    else pos -= arr[pos];
    if(pos < 0 || pos >= n)
      break;
    if(check[pos]){
      cout << "INFINITE\n";
      return 0;
    }
    check[pos] = 1;
  }
  cout << "FINITE\n";
  return 0;
}
