#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

int main(){

  ifstream ifs("SampleDataset1/SampleDataset1.json");
  Json::Reader reader;
  Json::Value obj;
  reader.parse(ifs, obj); // reader can also read strings
  cout << "Type: " << obj["type"].asString() << endl;

}
