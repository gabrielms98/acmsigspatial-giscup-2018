#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>

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

class Edge{
  friend Geometry;
public:
  Edge();
  Edge(const Json::Reader reader);

private:
  int viaNetworkSourceId;
  string viaGlobalId;
  string viaAssetGroupName;
  Geometry viaGeometry;

  int fromNetworkSourceId;
  string fromGlobalId; //nome do vertice u
  string fromAssetGroupName;
  Geometry fromGeometry;

  int toNetworkSourceId;
  string toGlobalId; //nome do vertice v
  string toAssetGroupName;
  Geometry toGeometry;
};
