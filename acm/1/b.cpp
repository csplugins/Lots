#include <iostream>
#include <algorithm>

using namespace std;

int main(){
  int n,m;
  cin >> n >> m;
  int set1[n];
  int set2[m];
  int set3[n];

  for(int i = 0; i < n; i++){
    cin >> set1[i];
    set3[i] = set1[i];
  }
  for(int i = 0; i < n; i++){
    cin >> set2[i];
  }
  sort(set1, set1+n);
  sort(set2, set2+m);
  if(set1[0] != set2[0]){
    cout << "Impossible" << endl;
    return 0;
  }
  for(int i = 1; i != n; i++){
    if(set1[i] != set2[i]){
      cout << "Impossible" << endl;
      return 0;
    }
    if(set1[i] == set1[i-1]){
      cout << "Ambiguity" << endl;
      return 0;
    }
  }
  cout << "Possible" << endl;
  for (int i = 0; i < n; i++)
    cout << set3[i] << " ";
  cout << endl;
  return 0;
}
