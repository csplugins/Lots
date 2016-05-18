#include <iostream>

using namespace std;

int main(){
  int digits, lo, hi;
  int returnMax = 2;
  int tempMax = 2;
  int index = 1;
  cin >> digits;
  int array[digits];
  for(int i = 0; i < digits; i++)
    cin >> array[i];

  hi = array[0];
  lo = array[1];
  while(hi == lo && index < digits){
    index++;
    if(array[index] != hi)hi = array[index];
    tempMax++;
  }
  for(int i = index; i < digits; i++){
    if(array[i] != hi || array[i] != lo){
      if(tempMax > returnMax) returnMax = tempMax;
      tempMax = 2;
      hi=array[i];
      lo=array[i-1];
      int tempIndex = i - 2;
      while(array[tempIndex] == lo || array[tempIndex] == hi){
	tempMax++;
	tempIndex--;
      }
    }else tempMax++;
  }
  if(tempMax > returnMax)
    returnMax = tempMax;
  cout << returnMax;
  return 0;
}
