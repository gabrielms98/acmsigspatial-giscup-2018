#include <iostream>
#include <map>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex;

int main(){
  map<string, int>dict;
  Graph g;
  ifstream ifs("SampleDataset1/SampleDataset1.json");
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj); // reader can also read strings

  int k=0;//sera o indice de cada vertice no grafo
  for(Json::Value val : obj["rows"]){//para cada vertice do json
    string u = val["fromGlobalId"].asString();
    string v = val["toGlobalId"].asString();
    if(dict.insert(make_pair(u, k)).second){//se o vertice do qual a aresta parte ainda nao tiver sido adicionado ao dict
       boost::add_vertex(g);
       k++;
    }
    if(dict.insert(make_pair(v, k)).second){//se o vertice no qual a aresta chega ainda nao tiver sido adicionado ao dict
      boost::add_vertex(g);
      k++;
    }
    boost::add_edge(dict[u],dict[v],g);
  }

  vector<bool>control(k,false);//vetor que indica quais vertices sao controladores
  for(Json::Value val : obj["controllers"]){//para cada controlador
    if(dict.insert(make_pair(val["globalId"].asString(), k)).second){//se ele ainda nao tiver sido adicionado ao dict
      boost::add_vertex(g);
      k++;
    }
    control[dict[val["globalId"].asString()]] = true;
  }

  vector<bool>start(k,false);//vetor que indica quais vertices sao starting points
  ifstream startingpoints("SampleDataset1/startingpoints.txt");
  while(true){
    string st;
    startingpoints >> st;
    if(dict.insert(make_pair(st, k)).second){
      boost::add_vertex(g);
      k++;
    }
    start[dict[st]] = true;
    if(startingpoints.eof())break;
  }

  vector<vertex> art_points;
  boost::articulation_points(g, back_inserter(art_points));
  cerr << "Found " << art_points.size() << " articulation points.\n";

 }
