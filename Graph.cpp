#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "Graph.h"

//======== FUNCOES PATH ========

Path::Path(const Path &p){
  this->x = p.x;
  this->y = p.y;
  this->z = p.z;
  this->m = p.m;
}

Path::Path(const double x, const double y, const double z, const double m){
  this->x = x;
  this->y = y;
  this->z = z;
  this->m = m;
}

//======== FUNCOES GEOMETRY ========

Geometry::Geometry(const Geometry &geo){
  this->hasM = geo.hasM;
  this->hasZ = geo.hasZ;
  this->path = geo.path;
}

//======== FUNCOES VERTICE ========
Vertice::Vertice(){
  this->NetworkSourceId = 0;
  this->GlobalId = " ";
  this->AssetGroupName = " ";
  this->TerminalId = 0;
  this->Geometry_ = Geometry();
}

Vertice::Vertice(const Vertice &v){
  this->NetworkSourceId = v.NetworkSourceId;
  this->GlobalId = v.GlobalId;
  this->AssetGroupName = v.AssetGroupName;
  this->TerminalId = v.TerminalId;
  this->Geometry_ = Geometry(v.Geometry_);
}

//======== FUNCOES EDGE ========
Edge::Edge(){
  this->AssetGroupName = " ";
  this->NetworkSourceId = 0;
  this->Geometry_ = Geometry();
  this->v = Vertice();
  this->u = Vertice();
}

Edge::Edge(const Edge &e){
  this->AssetGroupName = e.AssetGroupName;
  this->NetworkSourceId = e.NetworkSourceId;
  this->Geometry_ = Geometry(e.Geometry_);
  this->v = Vertice(e.v);
  this->u = Vertice(e.u);
}

//======== FUNCOES SOURCE MAPPING ========
sourceMapping::sourceMapping(const sourceMapping &s){
  this->dict = map<int, string>(s.dict);
}

void sourceMapping::setDict(const int i, const string st){
  this->dict[i] = st;
}

//======== FUNCOES GRAPH ========
Graph::Graph(const Graph &g){
  this->type = g.type;
  this->src = sourceMapping(g.src);
}
