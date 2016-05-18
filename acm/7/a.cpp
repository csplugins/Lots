#include <iostream>
////
using namespace std;

int main(){
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n, m, l, r, negative1, positive1;
  negative1 = 0;
  positive1 = 0;
  cin >> n >> m;
  for(int i = 0; i < n; i++){
    int temp;
    cin >> temp;
    if(temp == 1)
      positive1++;
    else negative1++;
  }
  /*while(m > 0){
    m--;*/
  for(int i = 0; i < m; ++i){
    cin >> l >> r;
    int range = r - l + 1;

    if( (range%2) == 1)
      cout << 0 << "\n";
    else if(negative1 >= range/2 && positive1 >= range/2)
      cout << 1 << "\n";
    else cout << 0 << "\n";
  }
}
