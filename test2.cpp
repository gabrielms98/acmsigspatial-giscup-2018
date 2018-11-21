#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include "boost/graph/graph_traits.hpp"

using namespace std;
typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

void saveG(const Graph &g, const string &file, vector<bool> &start, vector<bool>&control, vector<bool>&art){
	  ofstream f(file.c_str(),ios::out);
		f << "graph G{ " << '\n';
		for(int i=0;i<start.size();i++){
			f << i;
      if(art[i]) f << "[color=\".7 .3 1.0\"]" << "[style=filled]";
      if(start[i]) f << "[shape=triangle]";
      else if(control[i]) f << "[shape=box]";
      f << ";" << '\n';
    }
    //pair<edge_iterator, edge_iterator> ei = boost::edges(g);
    pair<edge_iterator, edge_iterator> ei;
    for(ei = boost::edges(g); ei.first != ei.second; ++ei.first)
		  f << source(*ei.first, g) << " -- " << target(*ei.first, g) << ";\n";
	f << "}" <<endl;
	f.close();
}

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

  vector<vertex> art_pointsAUX;
  vector<bool> art_points(k, false);
  boost::articulation_points(g, back_inserter(art_pointsAUX));
  for(int i=0; i<art_pointsAUX.size(); i++) art_points[(int)art_pointsAUX[i]]=true;
  saveG(g,"vaidamerda.dot",start,control,art_points);
 }
