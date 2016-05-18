#include <iostream>

using namespace std;

int main(){
  int n;
  int result = 0;
  int x = 0;
  int y = 0;
  cin >> n;
  string str;
  cin >> str;
  char commandArray[n];
  for(int i = 0 ; i < n; i++){
    commandArray[i] = str[i];
    if(commandArray[i] == 'U') y++;
    else if(commandArray[i] == 'D')y--;
    else if(commandArray[i] == 'L')x--;
    else x++;
  }
  int x2 = x;
  int y2 = y;
  for(int i = 0; i < n; i++){
    for(int j = i; j < n; j++){
          if(commandArray[j] == 'U') y2++;
	  else if(commandArray[j] == 'D')y2--;
	  else if(commandArray[j] == 'L')x2--;
	  else x2++;
	  if(x == x2 && y == y2) result++;
    }
    x2 = x;
    y2 = y;
  }
  cout << result;
  return 0;
}
