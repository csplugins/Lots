#include<iostream>
#include <map>

using namespace std;

int main(){
  int n;
  int answer =0;
  cin >> n;
  string arr[n];
  for(int i = 0; i < n; ++i)
    arr[i] = "";
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j){
      char temp;
      cin >> temp;
      arr[i] += temp;
    }

  map<string, int> m;
  for(int i = 0; i < n; ++i)
    if(m.insert(make_pair(arr[i], 1)).second == false){
      m.find(arr[i])->second += 1;
      //      iter->second += 1;
    }
  
  for(int i = 0; i < m.size(); ++i){
    int num = m.find(arr[i])->second;
    if(num > answer)
      answer = num;
  }
  cout << answer;
  return 0;
}
