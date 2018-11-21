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

void saveG(const Graph &g, const string &file, vector<bool> &startBool, const vector<bool>&controlBool, const vector<bool>&arti){
	  ofstream f(file.c_str(),ios::out);
		f << "graph G{ " << '\n';
		for(int i=0;i<startBool.size();i++){
			f << i;
      if(arti[i]) f << "[color=\".7 .3 1.0\"]" << "[style=filled]";
      if(startBool[i]) f << "[shape=triangle]";
      else if(controlBool[i]) f << "[shape=box]";
      f << ";" << '\n';
    }
    pair<edge_iterator, edge_iterator> ei;
    for(ei = boost::edges(g); ei.first != ei.second; ++ei.first)
		  f << source(*ei.first, g) << " -- " << target(*ei.first, g) << ";\n";
	f << "}" <<endl;
	f.close();
}

void leGrafo(Graph &g, const string &js, const string &txt, vector<bool> &startBool, vector<bool> &controlBool, map<string, int> &dict){
	ifstream ifs(js);
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

	controlBool = vector<bool>(k,false);
  for(Json::Value val : obj["controllers"]){//para cada controlador
    if(dict.insert(make_pair(val["globalId"].asString(), k)).second){//se ele ainda nao tiver sido adicionado ao dict
      boost::add_vertex(g);
      k++;
    }
    controlBool[dict[val["globalId"].asString()]] = true;
	}

  ifstream startingpoints(txt);
	startBool = vector<bool>(k,false);
  while(true){//para cada starting point
    string st;
    startingpoints >> st;
    if(dict.insert(make_pair(st, k)).second){
      boost::add_vertex(g);
      k++;
    }
    startBool[dict[st]] = true;
    if(startingpoints.eof())break;
  }
}

void addAux(Graph &g, const vector<bool> &startBool, const vector<bool> &controlBool){
	for(int i=0; i<startBool.size(); i++)
		if(startBool[i])
			for(int j=0; j<controlBool.size(); j++)
				if(controlBool[j]) boost::add_edge(i,j,g);
}

int main(int argc, char **argv){
  map<string, int>dict;
  Graph g;
	vector<bool>controlBool;//vetor que indica quais vertices sao controladores
	vector<bool>startBool;//vetor que indica quais vertices sao starting points
	leGrafo(g,argv[1],argv[2],controlBool,startBool,dict);
	addAux(g,startBool,controlBool);
  vector<vertex> arti;
  vector<bool> artiBool(startBool.size(), false);
  boost::articulation_points(g, back_inserter(arti));
  for(int i=0; i<arti.size(); i++) artiBool[(int)arti[i]]=true;
  saveG(g,"sampleDataset1.dot",startBool,controlBool,artiBool);
 }
