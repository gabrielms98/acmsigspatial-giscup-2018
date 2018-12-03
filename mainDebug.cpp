#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <jsoncpp/json/json.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/connected_components.hpp>
#include "boost/graph/graph_traits.hpp"
#include <string>
#include <omp.h>

using namespace std;

struct edge_component_t{//struct necessaria para o funcionamento da funcao biconnected_components
  enum
  { num = 555 };
  typedef boost::edge_property_tag kind;
}
edge_component;

typedef boost::adjacency_list<boost::listS,boost::vecS,boost::undirectedS,boost::no_property, boost::property <edge_component_t, std::size_t > > Graph;//lista de adjacencia representando o grafo do problema
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;//iterador que ira iterar sobre as arestas

void leGrafo(Graph &g, const string &js, const string &txt, vector<bool> &startVec, vector<bool>&controlVec, map<string, int> &vDictIN, map<int, string> &vDictOUT, multimap<string, int> &eDictIN, multimap<string, pair<int,int> > &edgesMapIN, map<pair<int,int>, string> &edgesMapOUT, int &V, int &E){

	ifstream ifs(js);//arquivo json
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj);

  for(Json::Value val : obj["rows"]){//para cada row do json
    string u = val["fromGlobalId"].asString();//vertice de partida
    string v = val["toGlobalId"].asString();//vertice de chegada
		string e = val["viaGlobalId"].asString();//via

    if(vDictIN.insert(make_pair(u,V)).second){//para o vertice from, se ele ainda nao tiver sido inserido:
      boost::add_vertex(g);
      vDictOUT.insert(make_pair(V++,u));//inserimos o vertice no dicionario de saida e incrementamos a variavel de indice
    }

    if(vDictIN.insert(make_pair(v,V)).second){//para o vertice to, se ele ainda nao tiver sido inserido:
      boost::add_vertex(g);
      vDictOUT.insert(make_pair(V++,v));//inserimos o vertice no dicionario de saida e incrementamos a variavel de indice
    }

    eDictIN.insert(make_pair(e,E++));//teremos que inserir a aresta de qualquer forma, pois nao ha arestas repetidas no json. Incrementamos a variavel de indice de aresta
    boost::add_edge(vDictIN[u], vDictIN[v], g);//ligue a aresta ao vertice u e ao vertice v
    edgesMapIN.insert(make_pair(e, make_pair(vDictIN[u], vDictIN[v])));//inserimos a aresta no dicionario de entrada
    edgesMapOUT.insert(make_pair(make_pair(vDictIN[u], vDictIN[v]), e));//inserimos a aresta no dicionario de saida
  }

  controlVec = vector<bool>(V,false);//vector que informara' quais vertices sao controladores
  startVec = vector<bool>(V,false);//vector que informara' quais vertices sao starting points

  for(Json::Value val : obj["controllers"]){//para cada controlador
    string c = val["globalId"].asString();
    controlVec[vDictIN[c]] = true;//setamos o controlvec na posicao daquele vertice para true
    vDictOUT[vDictIN[c]]=c;//inserimos seu nome no dicionario de saida
  }

  ifstream startingpoints(txt);//arquivo que contem os starting points
  while(true){//para cada starting point
    string st;
    startingpoints >> st;
    if(st.empty()) break;
    if(eDictIN.find(st)!=eDictIN.end()){//se o starting point for uma aresta
      boost::add_vertex(g);//criamos um vertice auxiliar que ira representa'-la
      multimap<string, pair<int,int> >::iterator it = edgesMapIN.find(st);
      boost::add_edge(V, (*it).second.first, g);//:
      boost::add_edge(V, (*it).second.second, g);//conectamos ele aos dois vertices aos quais a aresta que e' starting point estaria conectada
      startVec[V++] = true;//informamos que o vertice e' um starting point e incrementamos a variavel de indice
    }
    else{//se o starting point for um vertice
      startVec[vDictIN[st]] = true;
    }
    if(startingpoints.eof())break;
  }
}

void addAux(Graph &g, const vector<bool> &startVec, const vector<bool> &controlVec, const vector<int> &cc, int numCC, set<pair<int,int> > &auxEdges, int &V){
  boost::add_vertex(g);//criamos um vertice que sera ligado a todos os starting points
  int s = V++;//pegamos o indice dele e incrementamos a variavel de indice
  bool entrouS=false;//indica se teve pelo menos um starting point
  // #pragma omp parallel for
  for(int i=0; i<V-1; i++)//para cada vertice do CC (tirando o que acabou de ser criado (por isso o "-1") ), se ele for starting point, conecte-o ao vertice que acabou de ser criado (s)
    if(startVec[i]&&cc[i]==numCC){//se o vertice for starting point e for do componente conexo passado como argumento, crie a aresta.
      entrouS=true;
      add_edge(i, s, g);//adiciona uma aresta entre tal starting point e o vertice que foi criado
    }

  boost::add_vertex(g);//criamos um vertice que sera ligado a todos os starting points
  int c = V++;//pegamos o indice dele e incrementamos a variavel de indice
  bool entrouC=false;//indica se teve pelo menos um controlador
  // #pragma omp parallel for
  for(int i=0; i<V-2; i++)//para cada vertice do grafo(tirando os que acabaramde ser criados (por isso o "-2") ), se ele for controller, conecte-o ao vertice que acabou de ser criado (c)
    if(controlVec[i]&&cc[i]==numCC){//se o vertice for controller e for do componente conexo passado como argumento, crie a aresta.
      entrouC=true;
      add_edge(i, c, g);//adiciona uma aresta entre tal controller e o vertice que foi criado
    }

  if(entrouS&&entrouC){//se tiver encontrado pelo menos um starting point e um controller
    add_edge(s, c, g);//conecte os dois vertices auxiliares s e c.
    auxEdges.insert(make_pair(s,c));//insira a aresta no container de arestas auxiliares
  }
}

int main(int argc, char **argv){
  Graph g;
  map<string, int> vDictIN;//dicionario que ira converter o nome de cada vertice para seu indice
  map<int, string> vDictOUT;//dicionario que ira converter o indice de cada vertice para seu nome
  multimap<string, int> eDictIN;//dicionario que ira converter o nome de cada aresta para seu indice
  multimap<string, pair<int,int> > edgesMapIN;//dicionario que ira converter o nome de cada aresta para um pair com seus vertices
  map<pair<int,int>, string> edgesMapOUT;//dicionario que ira converter os vertices da aresta para seu nome
  ofstream out("output.txt");
	int V=0;//representara o indice de cada vertice
  int E=0;//representara o indice de cada aresta
  vector<bool>controlVec;//vector que indica quais vertices sao controladores
	vector<bool>startVec;//vector que indica quais vertices sao starting points

  leGrafo(g,argv[1],argv[2],startVec,controlVec,vDictIN,vDictOUT,eDictIN,edgesMapIN,edgesMapOUT,V,E);
  vector<int>cc(V);//armazena, no indice de cada vetor, o componente conexo ao qual ele pertence
  set<int> ccNums;//armazena o numero dos componentes conexos
  boost::connected_components(g, &cc[0]);//funcao que armazena em cc o numero do componente conexo de cada vertice
  for(int i=0; i<V; i++)//armazena todos os numeros de componentes conexos em ccNums
    ccNums.insert(cc[i]);

  set<pair<int,int> > auxEdges;//armazenara todas as arestas que forem auxiliares
  for(set<int>::iterator it = ccNums.begin(); it!=ccNums.end(); it++)//para cada componente conexo:
    addAux(g,startVec,controlVec,cc,(*it),auxEdges,V);//crie a aresta auxiliar nele

  boost::property_map<Graph, edge_component_t>::type component = get(edge_component, g);//necessario para o funcionamento da funcao biconnected_components
  boost::biconnected_components(g, component);//armazena em component o numero do componente biconectado de cada aresta

  set<int> solComps;//armazena o numero dos componentes biconectados que fazem parte da solucao
  for(pair<edge_iterator, edge_iterator> ei = boost::edges(g); ei.first != ei.second; ++ei.first){//para cada aresta, verifique se ela e' auxiliar. caso seja, insira o numero de seu componente biconectado em solComps
    //ps: nao podemos iterar apenas sobre auxEdges pois 'component', criado com a boost, requer um iterador para arestas, e nao apenas os vertices 'u' e 'v'
    int u = source(*ei.first,g);//pega o indice do vertice do qual a aresta sai
    int v = target(*ei.first,g);//pega o indice do vertice para o qual a aresta vai
    if(auxEdges.find(make_pair(u,v))!=auxEdges.end()){//se a aresta for auxiliar:
      solComps.insert(component[*ei.first]);//adicione seu componente biconectado 'a solucao
    }
  }

  for(pair<edge_iterator, edge_iterator> ei = boost::edges(g); ei.first != ei.second; ++ei.first){//para cada aresta do grafo
    if(solComps.find(component[*ei.first])!=solComps.end()){//se ela pertencer a algum dos componentes biconectados da solucao:
      int u = source(*ei.first,g);
      int v = target(*ei.first,g);
      if(!vDictOUT[u].empty()) out << vDictOUT[u] << '\n';//:
      if(!vDictOUT[v].empty()) out << vDictOUT[v] << '\n';//imprima o nome de seus dois vertices
      if(!edgesMapOUT[make_pair(u,v)].empty()) out << edgesMapOUT[make_pair(u,v)] << '\n';//imprima seu proprio nome
    }
  }
  return 0;
}
