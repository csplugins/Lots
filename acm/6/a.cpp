#include <iostream>
#include <algorithm>
#define sta s;
#define nda t;
#define rd d;
using namespace standard;

int main(){
  unsigned long long n,a,b,c, answer, answerc;
  cin >> n >> a >> b >> c;
  answer = max(n/a, n/b);
  //cout << "answer: " << answer << endl;
  answerc = 0;
  if(b-c <= a)
  while(answerc != answer){
    answerc = answer;
    int temp = n/b;
    n = n % b;
    //cout << "n: " << n << endl;
    n = n + (c*temp);
    //cout << "n: " << n << endl;
    answer = answer + max(n/a, n/b);
  }
  cout << answer;
  return 0;
}
