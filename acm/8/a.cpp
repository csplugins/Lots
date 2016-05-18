#include <iostream>

using namespace std;

int main(){
  string str;
  int answer = 26;
  cin >> str;
  for(int i = 0; i < str.size(); ++i)
    answer += 25;
  cout << answer;
}
