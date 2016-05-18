#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
  int answer = 0;
  int n;
  cin >> n;
  vector<int> v;
  for(int i = 0;i < n; ++i){
    int temp;
    cin >> temp;
    v.push_back(temp);
  }
  sort(v.begin(), v.end());
  for(int i = 0; i < v.size()-1; ++i){
    if(v[i] > v[i+1])continue;
    if(v[i] < v[i+1]) answer++;
    else{
      for(int j = i+1; j < v.size()-1; ++j)
        if(v[j] != v[j+1]){
          v.push_back(v[i]);
          break;
        }
    }
  }
  cout << answer;
  return 0;
}
