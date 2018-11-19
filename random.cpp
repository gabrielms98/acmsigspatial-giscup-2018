#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include "Graph.h"

using namespace std;

void tarjanUT(int u, const vector<list<int> > &adj, vector<int> &dfsParent, vector<int> &dfsNum,
					 vector<int> &dfsLow, vector<bool> &isArticulation,	int &dfsCount, int &rootChildren, int dfsRoot) {
	dfsLow[u] = dfsNum[u] = dfsCount++; // dfsLow[u] <= dfsNum[u]
	for(list<int>::const_iterator it = adj[u].begin(); it!=adj[u].end(); it++) {
		//vertices nao descobertos tem tempo de descoberto (dfsNum) 0
		if (dfsNum[(*it)] == 0) {
				dfsParent[(*it)] = u; //pai de v e' u (v foi descoberto por u)
			if (u == dfsRoot) //caso especial... precisamos contar quantas vezes descobrimos vertices a partir da raiz para descobrir se raiz e' articulacao...
				rootChildren++;

			tarjanUT((*it),adj,dfsParent,dfsNum,dfsLow,isArticulation,dfsCount,rootChildren,dfsRoot);

			if (dfsLow[(*it)] >= dfsNum[u]) //apos visitar v recursivamente, conseguimos chegar em um vertice em v ou antes passando por u? (sem voltar pela aresta v,u)
				isArticulation[u] = true; // store this information first
			dfsLow[u] = min(dfsLow[u], dfsLow[(*it)]); // update dfsLow[u]
		}
		else if ((*it) != dfsParent[u]) //cheguei de volta em um vertice ja descoberto... se nao passei pela aresta v,u de volta --> ciclo...
			dfsLow[u] = min(dfsLow[u], dfsNum[(*it)]); //atualize dfsLow...
	}
}

void tarjan(const vector<list<int> > &adj, vector<bool> &isArticulation) {
	int nv = adj.size();
	vector<int> dfsParent(nv,-1);
	vector<int> dfsLow(nv,0);
	vector<int> dfsNum(nv,0);
	int dfsCount = 0;

	//para multiplos CCs ...
	for(int i=0; i<nv; i++) {
		if(dfsNum[i]==0) {
			int rootChildren = 0;//resete para cada CC!!
			tarjanUT(i,adj,dfsParent,dfsNum,dfsLow,isArticulation,dfsCount,rootChildren,i);
			isArticulation[i] = rootChildren>1;
		}
	}
}


void geraGrafo(Graph &g, int v, int e) {
	g = Graph(v);
	srand(time(NULL));
	int currE = g.E();
	for(int i=0; i<e; i++) {
		int x = rand()%v;
		int y = rand()%v;
		g.insert(x,y);
		if(g.E()==currE) i--;
	}
}

int main(int argc, char **argv){
	int v = atoi(argv[2]);
	Graph g(v);
	geraGrafo(g, v, atoi(argv[3]));

	vector<bool>start(v);
	vector<bool>control(v);
	vector<bool>sol(v); for(int i=0; i<v; i++){sol[i] = false; control[i] = false; start[i] = false;}

	/*start[1] = true;
	control[v-1] = true;
	control[v-3] = true;*/
	tarjan(g.adj, sol);

	g.save(argv[1], start, control, sol);
}
