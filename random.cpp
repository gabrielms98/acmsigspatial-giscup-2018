#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include "Graph.h"

using namespace std;

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
	vector<bool>sol(v); for(int i=0; i<v; i++){sol[i] = false;control[i] = false;start[i] = false;}

	start[1] = true;
	control[v-1] = true;
	control[v-3] = true;
	for(int i=0; i<v; i++) if(i%2==0)sol[i]=true;

	g.save(argv[1], start, control, sol);
}
