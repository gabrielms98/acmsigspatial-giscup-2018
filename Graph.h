#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>

#ifndef GRAPH_H
#define GRAPH_H

using namespace std;

class Path{
public:
  double x;
  double y;
  double z;
  double m;
};

class Geometry{
  friend Path;
public:
  bool hasZ;
  bool hasM;
  Path *path;
};

struct Vertice{
  friend Geometry;
  int NetworkSourceId;
  string GlobalId; //nome do vertice u
  string AssetGroupName;
  Geometry Geometry;
};

class Edge{
  friend Geometry;
  friend Vertice;
public:
  Edge();
  Edge(const Json::Value obj);

private:
  int viaNetworkSourceId;
  string viaGlobalId;
  string viaAssetGroupName;
  Geometry viaGeometry;

  Vertice u;
  Vertice v;
};

struct sourceMapping{
  map<int, string> src;

  sourceMapping(int i, string st) { src[i] = st; }
};

class Graph{
  friend Edge;
  friend sourceMapping;

private:
  string type;
  sourceMapping src;

  vector<Vertice> *controllers;
  vector<list<int> > adj; 
};

#endif
