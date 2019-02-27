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
#include "include/rapidjson/document.h"

using namespace std;
using namespace rapidjson;

int main(){
  string str;
  getline(cin,str,(char)-1);
  Document doc;
  doc.Parse(str.c_str());

  assert(doc.IsObject());
  assert(doc.HasMember("rows"));
  auto &rows = doc["rows"];
  assert(rows.IsArray());
  for(auto &a : rows.GetArray()){
    assert(a.IsObject());

    assert(a["toGlobalId"].IsString());
    assert(a["fromGlobalId"].IsString());
    assert(a["viaGlobalId"].IsString());

    string u = a["fromGlobalId"].GetString();
    string v = a["toGlobalId"].GetString();
    string e = a["viaGlobalId"].GetString();

    cout << u << " -- " << v << " : " << e << '\n';
  }

}
