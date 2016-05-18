#include <iostream>

using namespace std;
int main(){
  int digits1, digits2, base1, base2;
  long long num1=0, num2=0;
  cin >> digits1 >> base1;
  for(int i = 0; i < digits1; i++){
    int temp;
    cin >> temp;
    num1 = num1*base1 + temp;
  }
  cin >> digits2 >> base2;
  for(int i = 0; i < digits2; i++){
    int temp;
    cin >> temp;
    num2 = num2*base2 + temp;
  }
  if(num1 == num2)
    cout << "=" << endl;
  else if(num1 < num2)
    cout << "<" << endl;
  else
    cout << ">" << endl;

  return 0;
}
