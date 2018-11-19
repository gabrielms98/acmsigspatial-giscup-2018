#include <iostream>
#include <map>
#include "Graph.h"
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

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

 vector<bool>control(k+1,false);//vetor que indica quais vertices sao controladores
 for(Json::Value val : obj["controllers"]){//para cada controlador
   if(dict.insert(make_pair(val["globalId"].asString(), k)).second) k++;//se ele ainda nao tiver sido adicionado ao dict
   control[dict[val["globalId"].asString()]] = true;
 }

 vector<bool>start(k+1,false);//vetor que indica quais vertices sao starting points
 ifstream startingpoints("SampleDataset1/startingpoints.txt");
 while(true){
   string st;
   startingpoints >> st;
   if(dict.insert(make_pair(st, k)).second) k++;
   start[dict[st]] = true;
   if(startingpoints.eof())break;
 }

 Graph g(k+1);
 for(Json::Value val : obj["rows"]){
   int v, w;
   v = dict[val["fromGlobalId"].asString()];
   w = dict[val["toGlobalId"].asString()];
   g.insert(v,w);
 }

vector<bool>sol(k+1, false);
g.save("vaidamerda.dot",start, control, sol);
}
