#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <jsoncpp/json/json.h>

#ifndef GRAPH_H
#define GRAPH_H

using namespace std;

class Path{
public:
  Path() : x(0), y(0), z(0), m(0) {}
  Path(const Path&);
  Path(const double, const double, const double, const double);

  double getX() const {return x;}
  double getY() const {return y;}
  double getZ() const {return z;}
  double getM() const {return m;}

private:
  double x;
  double y;
  double z;
  double m;
};

class Geometry{
  friend Path;
public:
  Geometry() : hasM(false), hasZ(false), path(0) {}
  Geometry(const Geometry&);
  Geometry(const bool m_, const bool z_, const vector<Path>&path_):
    hasM(m_), hasZ(z_), path(path_){}

  int sizePath() const{return path.size();}
  bool getHasZ() const{return hasZ;}
  bool getHasM() const{return hasM;}
  vector<Path> getPathCopy() const{return vector<Path>(path);}

private:
  bool hasZ;
  bool hasM;
  vector<Path> path;
};

class Vertice{
  friend Geometry;

public:
  Vertice();
  Vertice(const int nt, const string g, const string asst, const int t, const Geometry &geo) :
    NetworkSourceId(nt), GlobalId(g), AssetGroupName(asst), TerminalId(t), Geometry_(geo) {}
  Vertice(const Vertice&);

  int getNetworkSourceId()const {return NetworkSourceId;}
  string getGlobalId()const {return GlobalId;}
  string getAssetGroupName()const {return AssetGroupName;}
  int getTerminalId()const {return TerminalId;}
  Geometry getGeometryCopy()const {return Geometry(Geometry_);}

private:
  int NetworkSourceId;
  string GlobalId; //nome do vertice u
  string AssetGroupName;
  int TerminalId;
  Geometry Geometry_;
};

class Edge{
  friend Geometry;
  friend Vertice;

public:
  Edge();
  Edge(const Edge&);
  Edge(const int nt, const string g, const string asst, const Geometry &geo, const Vertice &u_, const Vertice &v_) :
    NetworkSourceId(nt), GlobalId(g), AssetGroupName(asst), Geometry_(geo), u(u_), v(v_) {}

  int getNetworkSourceId()const { return NetworkSourceId;}
  string getGlobalId()const { return GlobalId;}
  string getAssetGroupName()const { return AssetGroupName;}
  Geometry getGeometryCopy()const { return Geometry(Geometry_);}
  Vertice getUCopy()const {return Vertice(u);}
  Vertice getVCopy()const {return Vertice(v);}

private:
  int NetworkSourceId;
  string GlobalId;
  string AssetGroupName;
  Geometry Geometry_;
  Vertice u;
  Vertice v;
};

struct sourceMapping{
  sourceMapping(){}
  void setMapping(const int, const string);
  private:
  map<int, string> src;
};

class Graph{
public:
  friend Vertice;
  friend Edge;
  friend sourceMapping;

private:
  string type;
  sourceMapping src;

  vector<Vertice> *controllers;
  vector<list<Vertice> > adj;
};

#endif
