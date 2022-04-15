#include "VRP.h"

using namespace std;

void VRP::read(string file) {
  ifstream f;
  f.open(file);
  string line;
  f >> line;
  f >> NoOfCustomers;
  f >> line;
  f >> NoOfVehicles;
  getline(f, line);
  getline(f, line);
  int i = 0;
  for (size_t i{0}; i != NoOfCustomers; ++i)
  {
    costMatrix.emplace_back(NoOfCustomers);
  }
  while (getline(f, line)) {
    vector<double> val = values(line);
    Node node(i,0);
    Nodes.push_back(node);
    for (int j = 0; j < val.size(); j++) {
      costMatrix[i][j] = val[j];
    }
    i++;
  }
  Nodes[0].setDepot(true);
}

std::vector<double> VRP::values(std::string line) {
  char space_char = ' ';
  vector<double> val;
  stringstream sstream(line);
  string number;
  while (getline(sstream, number, space_char)){
    val.push_back(stod(number));
  }
  return val;
}

void VRP::print() {
  cout << "Number of vehicles: " << NoOfVehicles << endl;
  cout << "Number of clients: " << NoOfCustomers << endl;
  for (int i = 0; i < costMatrix.size(); i++) {
    for (int j = 0; j < costMatrix.size(); j++) {
      cout << costMatrix[i][j];
    }
    cout << endl;
  }
}

bool VRP::UnassignedCustomerExists(vector<Node> nodes) {
  for (int i = 1; i < nodes.size(); i++) {
    if (!nodes[i].getRouted()) return true;
  }
  return false;
}