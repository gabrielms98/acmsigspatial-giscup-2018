#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Graph.h"
using namespace std;

Graph::Graph(int V) :
  adj(V), Vcnt(V), Ecnt(0){
  adj.resize(V);
}

int Graph::V() const { return Vcnt; }

int Graph::E() const { return Ecnt; }

void Graph::insert(int v, int w){
      if(find(adj[v].begin(), adj[v].end(), w)==adj[v].end()){
        adj[v].push_back(w);
        adj[w].push_back(v);
        Ecnt++;
      }
}

void Graph::remove(int v, int w) {
    for(list<int>::iterator it=adj[v].begin(); it!=adj[v].end(); it++)
  		if ((*it) == w) {
  			adj[v].erase(it);
  			break;
  		}
    for(list<int>::iterator it= adj[w].begin(); it!=adj[w].end();it++)
			if ((*it) == v) {
				adj[w].erase(it);
				break;
			}
}

bool Graph::edge(int v1, int w1) const {
	for(list<int>::const_iterator it = adj[v1].begin(); it!=adj[v1].end(); it++)
		if ((*it)==w1) return true;
	return false;
}

void Graph::save(const string &file, vector<bool>start, vector<bool>control, vector<bool>sol) const {
	ofstream f(file.c_str(),ios::out);

		f << "graph G{ " << '\n';
		for(int i=0;i<V();i++){
			f << i;
      if(sol[i]) f << "[color=\".7 .3 1.0\"]" << "[style=filled]";
      if(start[i]) f << "[shape=triangle]";
      else if(control[i]) f << "[shape=box]";
      f << ";" << '\n';
    }
		for(int i=0;i<V();i++) {
			for(list<int>::const_iterator it = adj[i].begin(); it != adj[i].end(); it++)
				if (i<(*it))
					f << i << " -- "<< (*it) << ";\n";
		}
	f << "}" <<endl;
	f.close();
}


/*Graph::Graph(const string &file) {
	ifstream fin(file.c_str(),ios::in);

	string str;
	fin >> str;

	if (str!="digraph" && str!="graph") {
		cerr << "Erro ao abrir arquivo dot! formato de arquivo invalido"<< endl;
		exit(0);
	}
	digraph = (str=="digraph");

	char c=' ';
	while(c!='{') {
		fin >> c;
		if (fin.eof()) {
			cerr << "Erro ao abir arquivo dot! eof alcançado";
			exit(0);
		}
	}

	string linha;
	vector<pair<int,int> > vt;
	Vcnt = 0;
	Ecnt =0;

	while(c!='\n') fin.get(c);
	while(true) {
		getline(fin,linha);
		stringstream st(linha);

		if (linha.find("}")!=string::npos) //Se estivermos no final do grafo..
			break;
		int a,b;
		if (linha.find("--")==string::npos && linha.find("->")==string::npos) {
			st >> a;
			Vcnt = max(Vcnt,a);
			continue;
		}

		st >> a >> str >> b;
		Vcnt = max(max(Vcnt,a),b);
		vt.push_back(pair<int,int>(a,b));

	}
	adj.resize(Vcnt);


	for(int i=0;i<vt.size();i++) {
		insert(Edge(vt[i].first-1,vt[i].second-1));
	}
	cerr << "Grafo carregado com " << V() << " vertices " << " e " << E() << " arestas " <<endl;
	fin.close();

}*/
