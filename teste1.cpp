#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "Graph.cpp"

using namespace std;

int main(){
  vector<Path> p(10);
  for(int i=0; i<10; i++){
    p[i] = Path(i,i,i,i);
  }
  Geometry g(true, true, p);
  int nt = 10;
  string ga = "paçoca";
  string asst = "asset";
  int t = 1;
  Vertice v(nt,ga,asst,t,g);

  for(int i=0; i<p.size(); i++)
    cout << v.getGeometryCopy().getPathCopy()[i].getX() << '\n';

}
