#include <iostream>
#include <set>

using namespace std;

int main(){
  int num1, num2, repeats;
  cin >> num1;
  cin >> num2;
  repeats = num2/2;
  for(int i = 0; i < repeats; i++){
    if((num1+1)*2 < (num2+1)*3)
      num1++;
    else{
      num2++;
      repeats = num2/2;
    }
  }
  if(num1*2 > num2*3)
    cout << num1*2;
  else     cout << num2*3;
  return 0;
}
