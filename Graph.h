#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <sstream>
#include <cstdlib>
using namespace std;

/*
"This code is from "Algorithms in C++, Third Edition,"
                by Robert Sedgewick, Addison-Wesley, 2002."
*/

#ifndef GRAPH_H
#define GRAPH_H

class Graph{
public:
  int Vcnt, Ecnt;// bool digraph;
  /*set<int> control;
  set<int> start;
  vector<bool> sol;*/
  vector <list <int> > adj;
  Graph(int V);
  //Graph(const string &file);
  int V() const;
  int E() const;

  void insert(int, int);
  void remove(int, int);
  bool edge(int, int) const;
  void save(const string &file, vector<bool>start, vector<bool>control, vector<bool>sol) const;
};

#endif
