#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include "boost/graph/graph_traits.hpp"
//#include <boost/config.hpp>

using namespace std;

struct edge_component_t{
  enum
  { num = 555 };
  typedef boost::edge_property_tag kind;
}
edge_component;

typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS,boost::no_property, boost::property <edge_component_t, std::size_t > > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_d;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;


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

void leGrafo(Graph &g, const string &js, const string &txt, set<int> &startSet, set<int>&controlSet, map<string, int> &vDictIn, map<string, int> &eDictIn, vector<string> &vDictOut, vector<string> &eDictOut, int &V, int &E){
	ifstream ifs(js);
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);
  //V sera o indice de cada vertice no grafo, e no fim representara a quantidade total de vertices
  for(Json::Value val : obj["rows"]){//para cada aresta do json
    string u = val["fromGlobalId"].asString();
    string v = val["toGlobalId"].asString();
		string e = val["viaGlobalId"].asString();
    if(vDictIn.insert(make_pair(u, V)).second){//se o vertice do qual a aresta parte ainda nao tiver sido adicionado ao vDictIn
      boost::add_vertex(g);
			vDictOut.push_back(v);
      V++;
    }
    if(vDictIn.insert(make_pair(v, V)).second){//se o vertice no qual a aresta chega ainda nao tiver sido adicionado ao vDictIn
      boost::add_vertex(g);
			vDictOut.push_back(v);
      V++;
    }
		eDictIn.insert(make_pair(e, E));
		boost::add_edge(vDictIn[u],vDictIn[v],g);
		eDictOut.push_back(e);
		E++;


		/*vDictIn.insert(make_pair(u, V)).second;
		boost::add_vertex(g);
		vDictOut.push_back(v);
		V++;
		vDictIn.insert(make_pair(v, V)).second;
		boost::add_vertex(g);
		vDictOut.push_back(v);
		V++;
		eDictIn.insert(make_pair(e,E)).second;
		boost::add_edge(vDictIn[u],vDictIn[v],g);
		eDictOut.push_back(e);
		E++;*/
  }

  for(Json::Value val : obj["controllers"]){//para cada controlador
    if(vDictIn.insert(make_pair(val["globalId"].asString(), V)).second){//se ele ainda nao tiver sido adicionado ao vDictIn
      boost::add_vertex(g);
      V++;
    }
    controlSet.insert(vDictIn[val["globalId"].asString()]);
	}

  ifstream startingpoints(txt);
  while(true){//para cada starting point
    string st;
    startingpoints >> st;
    if(vDictIn.insert(make_pair(st, V)).second){
      boost::add_vertex(g);
      V++;
    }
    startSet.insert(vDictIn[st]);
    if(startingpoints.eof())break;
  }
}

void addAux(Graph &g, const set<int> &startSet, const set<int> &controlSet, vector<pair<int,int> > &auxEdges){//adiciona as arestas auxiliares, que ligam os starting points aos controllers

	set<int>::iterator it1, it2;
	for(it1=startSet.begin(); it1!=startSet.end(); it1++)
		for(it2=controlSet.begin(); it2!=controlSet.end(); it2++){
			boost::add_edge((*it1),(*it2),g);
			auxEdges.push_back(make_pair((*it1),(*it2)));
		}
}

int main(int argc, char **argv){

  map<string, int>vDictIn;
	vector<string>vDictOut;
	map<string, int>eDictIn;
	vector<string>eDictOut;

	map<pair<int,int>,int>compEdges;

  Graph g;
	int V=0;//representara o indice de cada vertice, e no final representara o numero total de vertices
	int E=0;
	set<int>controlSet;//set que indica quais vertices sao controladores
	set<int>startSet;//set que indica quais vertices sao starting points

	set<int>vSol;
	set<int>eSol;

	leGrafo(g,argv[1],argv[2],startSet,controlSet,vDictIn,eDictIn,vDictOut,eDictOut,V,E);
	vector<pair<int,int> >auxEdges;
	addAux(g,startSet,controlSet,auxEdges);
  //vector<vertex_d> artiVec;
  //set<int> artiSet;
  //boost::articulation_points(g, back_inserter(artiVec)); for(int i=0; i<artiVec.size(); i++) artiSet.insert((int)artiVec[i]);
	boost::property_map<Graph, edge_component_t>::type component = get(edge_component, g);
	boost::biconnected_components(g, component);

  for (pair<edge_iterator, edge_iterator> ei = boost::edges(g); ei.first != ei.second; ++ei.first){
		compEdges.insert(make_pair(make_pair(source(*ei.first,g),target(*ei.first,g)),component[*ei.first]));
	}
  geraDot(g,"sampleDataset1.dot",startSet,controlSet,compEdges,V);
 }
