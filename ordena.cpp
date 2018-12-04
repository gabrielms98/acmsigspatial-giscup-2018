#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

int main(int argc, char **argv){
  ifstream ifs1(argv[1]);
  ifstream ifs2(argv[2]);
  string st1;
  string st2;
  vector<string> vec1;
  vector<string> vec2;
  while(true){
    if(ifs1.eof() && ifs2.eof()) break;
    ifs1 >> st1;
    vec1.push_back(st1);
    ifs2 >> st2;
    vec2.push_back(st2);
  }

  sort( vec1.begin(), vec1.end() );
  vec1.erase( unique( vec1.begin(), vec1.end() ), vec1.end() );
  sort( vec2.begin(), vec2.end() );
  vec2.erase( unique( vec2.begin(), vec2.end() ), vec2.end() );

  ofstream of1("ourOrd.txt");
  ofstream of2("expOrd.txt");

  for(string i : vec1) of1 << i << '\n';
  for(string i : vec2) of2 << i << '\n';

  cout << ((vec1==vec2) ? "True\n" : "False\n");

}
