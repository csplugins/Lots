#include <iostream>

using namespace std;
int main(){
  int questionMarks, adds, subtracts;
  int temp;
  string ops = "";
  cin >> temp;
  while(temp != '='){
    if(temp == '+')adds++;
    else if(temp == '-')subracts++;
    else questionMarks++;
    if(temp != '?')ops+=temp;
    cin >> temp;
  }
  adds++;
  if(subracts  > adds){
    cout << "Impossible" << endl;
    return 0;
  }
  cin >> temp;
  if(subtracts == 0 && temp < adds){
    cout << "Impossible" << endl;
    return 0;
  }
  for(int i = 1; i <= temp; ++i){
    
  }
  return 0;
}
