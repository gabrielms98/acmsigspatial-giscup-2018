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
#include <omp.h>

using namespace std;

struct edge_component_t{
  enum
  { num = 555 };
  typedef boost::edge_property_tag kind;
}
edge_component;

typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS,boost::no_property, boost::property <edge_component_t, std::size_t > > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

void geraGrafo(Graph &g, int v, int e, int s, int c, set<int> &startSet, set<int> &controlSet) {
	vector<pair<int,int> > arestas;
	srand(time(NULL));
	for(int i=0; i<v; i++)boost::add_vertex(g);
	for(int i=0; i<e;) {
		int x = rand()%v;
		int y = rand()%v;
		if(x!=y)
		if(find(arestas.begin(),arestas.end(),make_pair(x,y))==arestas.end() && find(arestas.begin(),arestas.end(),make_pair(y,x))==arestas.end()){
			boost::add_edge(x,y,g);
			arestas.push_back(make_pair(x,y));
			i++;
		}
	}
	for(int i=0; i<s;){
		int vv = rand()%v;
		if(startSet.insert(vv).second) i++;
	}
	for(int i=0; i<c;){
		int c = rand()%v;
		if(controlSet.insert(c).second && startSet.find(c)==startSet.end()) i++;
	}
}

void geraDot(const Graph &g, const string &file, const set<int> &startSet, const set<int>&controlSet, /*const set<int>&artiSet,*/ map<pair<int,int>, int> &compEdges, int V){
	  ofstream f(file.c_str(),ios::out);
		f << "graph G{ " << '\n';
		for(int i=0;i<V;i++){
			f << i;
			//if(solutSet.find(i)!=solutSet.end()) f << "[color=\".7 .3 1.0\"]" << "[style=filled]";//os vertices da solucao serao coloridos
      //if(artiSet.find(i)!=artiSet.end()) f << "[label = \"" << i <<"*\"]";//as articulacoes terao asterisco
      if(startSet.find(i)!=startSet.end()) f << "[shape=triangle]";//starting points terao forma de triangulo
      else if(controlSet.find(i)!=controlSet.end()) f << "[shape=box]";//controllers terao forma de quadrilatero
      f << ";" << '\n';
    }
    pair<edge_iterator, edge_iterator> ei;
    for(ei = boost::edges(g); ei.first != ei.second; ++ei.first){
		  f << source(*ei.first, g) << " -- " << target(*ei.first, g) << "[label = \"" << compEdges[make_pair(source(*ei.first, g), target(*ei.first, g))] << "\"];\n";

		}
	f << "}" <<endl;
	f.close();
}

void addAux(Graph &g, const set<int> &startSet, const set<int> &controlSet){

	set<int>::iterator it1, it2;
	for(it1=startSet.begin(); it1!=startSet.end(); it1++)
		for(it2=controlSet.begin(); it2!=controlSet.end(); it2++)
			boost::add_edge((*it1),(*it2),g);
}

int main(int argc, char **argv){
	int v = atoi(argv[1]);
	Graph g;
	set<int>startSet;
	set<int>controlSet;

	geraGrafo(g, v, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), startSet, controlSet);

	addAux(g,startSet,controlSet);
	map<pair<int,int>,int> compEdges;
	boost::property_map<Graph, edge_component_t>::type component = get(edge_component, g);
	boost::biconnected_components(g, component);

//#pragma omp parallel for
  for (pair<edge_iterator, edge_iterator> ei = boost::edges(g); ei.first != ei.second; ++ei.first){
		compEdges.insert(make_pair(make_pair(source(*ei.first,g),target(*ei.first,g)),component[*ei.first]));
	}
  geraDot(g,"randomG.dot", startSet, controlSet, compEdges, v);
}
