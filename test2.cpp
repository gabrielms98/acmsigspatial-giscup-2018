#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include "boost/graph/graph_traits.hpp"

using namespace std;
typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

void geraDot(const Graph &g, const string &file, const set<int> &startSet, const set<int>&controlSet, const set<int>&artiSet, int V){
	  ofstream f(file.c_str(),ios::out);
		f << "graph G{ " << '\n';
		for(int i=0;i<V;i++){
			f << i;
      if(artiSet.find(i)!=artiSet.end()) f << "[color=\".7 .3 1.0\"]" << "[style=filled]";
      if(startSet.find(i)!=startSet.end()) f << "[shape=triangle]";
      else if(controlSet.find(i)!=controlSet.end()) f << "[shape=box]";
      f << ";" << '\n';
    }
    pair<edge_iterator, edge_iterator> ei;
    for(ei = boost::edges(g); ei.first != ei.second; ++ei.first)
		  f << source(*ei.first, g) << " -- " << target(*ei.first, g) << ";\n";
	f << "}" <<endl;
	f.close();
}

void leGrafo(Graph &g, const string &js, const string &txt, set<int> &startSet, set<int>&controlSet, map<string, int> &dict, int &V){
	ifstream ifs(js);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj); // reader can also read strings

  //V sera o indice de cada vertice no grafo, e no fim representara a quantidade total de vertices
  for(Json::Value val : obj["rows"]){//para cada vertice do json
    string u = val["fromGlobalId"].asString();
    string v = val["toGlobalId"].asString();
    if(dict.insert(make_pair(u, V)).second){//se o vertice do qual a aresta parte ainda nao tiver sido adicionado ao dict
       boost::add_vertex(g);
       V++;
    }
    if(dict.insert(make_pair(v, V)).second){//se o vertice no qual a aresta chega ainda nao tiver sido adicionado ao dict
      boost::add_vertex(g);
      V++;
    }
    boost::add_edge(dict[u],dict[v],g);
  }

  for(Json::Value val : obj["controllers"]){//para cada controlador
    if(dict.insert(make_pair(val["globalId"].asString(), V)).second){//se ele ainda nao tiver sido adicionado ao dict
      boost::add_vertex(g);
      V++;
    }
    controlSet.insert(dict[val["globalId"].asString()]);
	}

  ifstream startingpoints(txt);
  while(true){//para cada starting point
    string st;
    startingpoints >> st;
    if(dict.insert(make_pair(st, V)).second){
      boost::add_vertex(g);
      V++;
    }
    startSet.insert(dict[st]);
    if(startingpoints.eof())break;
  }
}

void addAux(Graph &g, const set<int> &startSet, const set<int> &controlSet){

	set<int>::iterator it1, it2;
	for(it1=startSet.begin(); it1!=startSet.end(); it1++)
		for(it2=controlSet.begin(); it2!=controlSet.end(); it2++)
			boost::add_edge((*it1),(*it2),g);
}

int main(int argc, char **argv){
  map<string, int>dict;
  Graph g;
	int V=0;//representara o indice de cada vertice, e no final representara o numero total de vertices
	set<int>controlSet;//set que indica quais vertices sao controladores
	set<int>startSet;//set que indica quais vertices sao starting points
	leGrafo(g,argv[1],argv[2],startSet,controlSet,dict,V);
	//addAux(g,startSet,controlSet);
  vector<vertex> artiVec;
  set<int> artiSet;
  boost::articulation_points(g, back_inserter(artiVec)); for(int i=0; i<artiVec.size(); i++) artiSet.insert((int)artiVec[i]);
  geraDot(g,"sampleDataset1.dot",startSet,controlSet,artiSet,V);
 }
