#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include "boost/graph/graph_traits.hpp"

using namespace std;
typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

void geraGrafo(Graph &g, int v, int e) {
	vector<pair<int,int> > arestas;
	srand(time(NULL));
	for(int i=0; i<v; i++)boost::add_vertex(g);
	for(int i=0; i<e;) {
		int x = rand()%v;
		int y = rand()%v;
		if(x!=y && find(arestas.begin(),arestas.end(),make_pair(x,y))==arestas.end() && find(arestas.begin(),arestas.end(),make_pair(y,x))==arestas.end()){
			boost::add_edge(x,y,g);
			arestas.push_back(make_pair(x,y));
			i++;
		}
	}
}

void saveG(const Graph &g, const string &file, vector<bool> &startBool, vector<bool>&controlBool, vector<bool>&arti){
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

void addAux(Graph &g, const vector<bool> &startBool, const vector<bool> &controlBool){
	for(int i=0; i<startBool.size(); i++)
		if(startBool[i])
			for(int j=0; j<controlBool.size(); j++)
				if(controlBool[j]) boost::add_edge(i,j,g);
}

int main(int argc, char **argv){
	int v = atoi(argv[1]);
	Graph g;
	geraGrafo(g, v, atoi(argv[2]));

	vector<bool>startBool(v, false);
	vector<bool>controlBool(v, false);

	srand(time(NULL));
	startBool[rand()%v] = true;
	startBool[rand()%v] = true;
	controlBool[rand()%v] = true;
	controlBool[rand()%v] = true;

	vector<vertex> arti;
	vector<bool> artiBool(v, false);
	boost::articulation_points(g, back_inserter(arti));
	for(int i=0; i<arti.size(); i++) artiBool[(int)arti[i]]=true;
	addAux(g,startBool,controlBool);

  saveG(g,"randomG.dot", startBool, controlBool, artiBool);
}
