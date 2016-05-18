#include <iostream>

using namespace std;

int main(){
  int p1, p2;
  int charger;
  int minutes = 0;
  cin >> p1 >> p2;
  charger = p2 > p1;
  while(p1 > 0 && p2 > 0){
    if(charger){
      p1++;
      p2 = p2 - 2;
      if(p2 < 0)
        break;
    }
    else{
      p2++;
      p1 = p1 - 2;
      if(p1 < 0)
        break;
    }
    charger = p2 > p1;
    minutes++;
  }
  cout << minutes;
  return 0;
}
