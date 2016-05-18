#include <iostream>
using namespace std;

int main(){
  int d1,d2,d3;
  int total = 0;
  cin >> d1 >> d2>> d3;
  if (d1 < d3){
    total += d1;
    if(d1+d3 < d2)
      total += d1+d3+d3;
    else{
      total += d2;
      if(d3 < d2+d1)
	total += d3;
      else total += d2+d1;
    }
  }

  else{
    total+=d3;
    if(d3+d1 < d2)
      total += d3+d1+d1;
    else{
      total +=d2;
      if(d1 < d2+d3)
	total += d1;
      else total += d2+d3;
    }
  }
  cout << total << endl;
  return 0;
}
