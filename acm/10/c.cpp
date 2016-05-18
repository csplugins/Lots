#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int main(){
  int answer = 0;
  int n;
  cin >> n;
  int x[n], y[n];
  for(int i = 0; i < n; ++i){
    cin >> x[i] >> y[i];
  }
  for(int i = 0; i < n; ++i){
    for(int j = i+1; j < n; ++j){
      int t1 = x[i]-x[j];
      int t2 = y[i]-y[j];
      //if(t1 == 2 && t2 == 2){
        //answer++;
      //}
      /*else */if(t1 == 0 || t2 == 0)
        answer++;
    }
  }
  cout << answer << endl;
  return 0;
}
