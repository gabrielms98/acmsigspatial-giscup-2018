#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

int main(int argc, char **argv){
  ifstream ifs(argv[1]);
  string st;
  vector<string> vec;
  while(true){
    if(ifs.eof()) break;
    ifs >> st;
    vec.push_back(st);
  }

  sort( vec.begin(), vec.end() );
  vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

  for(string i : vec) cout << i << endl;

}
