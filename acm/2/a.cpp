#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(){
  int n;
  vector<int> vec;
  cin >> n;
  vec.push_back(1);
  for(int i = 1; i < n; i++){
    vec.push_back(1);
    while(true){
      if(vec[vec.size()-1] != vec[vec.size()-2])
	break;
      vec.pop_back();
      vec[vec.size()-1]++;
    }
    }
  for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";
  cout << endl;
  return 0;
}
