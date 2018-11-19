#include <iostream>
#include <map>
#include "Graph.h"
#include <fstream>
#include <jsoncpp/json/json.h>

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

int main(){
 map<string, int>dict;

 ifstream ifs("SampleDataset1/SampleDataset1.json");
 Json::Reader reader;
 Json::Value obj;
 reader.parse(ifs, obj); // reader can also read strings

 int k=0;//sera o indice de cada vertice no grafo
 for(Json::Value val : obj["rows"]){//para cada vertice do json
   if(dict.insert(make_pair(val["fromGlobalId"].asString(), k)).second) k++;//se o vertice do qual a aresta parte ainda nao tiver sido adicionado ao dict
   if(dict.insert(make_pair(val["toGlobalId"].asString(), k)).second) k++;//se no qual a aresta chega ainda nao tiver sido adicionado ao dict
 }

 vector<bool>control(k,false);//vetor que indica quais vertices sao controladores
 for(Json::Value val : obj["controllers"]){//para cada controlador
   if(dict.insert(make_pair(val["globalId"].asString(), k)).second) k++;//se ele ainda nao tiver sido adicionado ao dict
   control[dict[val["globalId"].asString()]] = true;
 }

 vector<bool>start(k,false);//vetor que indica quais vertices sao starting points
 ifstream startingpoints("SampleDataset1/startingpoints.txt");
 while(true){
   string st;
   startingpoints >> st;
   if(dict.insert(make_pair(st, k)).second) k++;
   start[dict[st]] = true;
   if(startingpoints.eof())break;
 }

 Graph g(k);
 for(Json::Value val : obj["rows"]){
   int v, w;
   v = dict[val["fromGlobalId"].asString()];
   w = dict[val["toGlobalId"].asString()];
   g.insert(v,w);
 }
 vector<bool>isArt(k, false);
 tarjan(g.adj, isArt);
 g.save("vaidamerda.dot",start, control, isArt);

}
