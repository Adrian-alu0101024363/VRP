#include "VRP.h"

using namespace std;

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' ');}

void VRP::read(string file) {
  ifstream f;
  f.open(file);
  string line;
  f >> line;
  f >> NoOfCustomers;
  NoOfCustomers++;
  f >> line;
  f >> NoOfVehicles;
  Vehicles.resize(NoOfVehicles);
  getline(f, line);
  getline(f, line);
  int i = 0;
  for (size_t i{0}; i != NoOfCustomers; ++i)
  {
    costMatrix.emplace_back(NoOfCustomers);
  }
  while (getline(f, line)) {
    string str = line;
    std::replace(std::begin(str),std::end(str),'\t',' ');
    std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
    str.erase(new_end, str.end());
    vector<double> val = values(str);
    Node node(i,0);
    Nodes.push_back(node);
    for (int j = 0; j < val.size(); j++) {
      //cout << val[j];
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
  cout << "Sad? " << UnassignedCustomerExists(Nodes);
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

void VRP::GreedySolution() {
  double CandCost, EndCost;
  int VehIndex = 0;
  for (int j = 0; j < NoOfVehicles; j++) {
    Vehicles[j].setCurrent(0);
  }
  int limit = (NoOfCustomers / NoOfVehicles) + 1; //cuidado con inpar
  int cont = 0;
  while (UnassignedCustomerExists(Nodes)) {
    int CustIndex = 0;
    Node Candidate;
    double minCost = (double)INT_MAX;
    /*if (Vehicles[VehIndex].getRoute().size() == 0) {
        //Vehicles[VehIndex].AddNode(Nodes[0]);
    }*/
    if (cont == limit) {
      Vehicles[VehIndex].AddNode(Nodes[0]);
      EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
      cost += EndCost;
      VehIndex++;
      Vehicles[VehIndex].AddNode(Nodes[0]);
      cont = 0;
    }
    for (int i = 0; i < NoOfCustomers; i++) {
      if (Nodes[i].getRouted() == false) {
        int pos = Vehicles[VehIndex].getCurrent();
        CandCost = costMatrix[pos][i];
        //if (Vehicles[VehIndex].CheckIfFits(Nodes[i].getDemand())) {
        //if (Vehicles[VehIndex].CheckIfFits(CandCost)) {
        if (minCost > CandCost) {
          cout << "En: " << pos + 1<< " : " << i + 1 << endl;
          cout << CandCost << endl;
          minCost = CandCost;
          CustIndex = i;
          Candidate = Nodes[i];
        }
        //}
      }
    }
    cout << "in node: " << Nodes[CustIndex].getId() +1 << endl;
    Vehicles[VehIndex].AddNode(Candidate);
    Nodes[CustIndex].setRouted(true);
    Vehicles[VehIndex].setCurrent(Nodes[CustIndex].getId());
    cost += minCost;
    cont++;
  }
  EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
  Vehicles[VehIndex].AddNode(Nodes[0]);
  cost += EndCost;
  cout << "Coste final: " << cost << endl;
  cout << "Ruta:" << endl;
  auto ruta = Vehicles[0].getRoute();
  for (int k = 0; k < ruta.size(); k++) {
    cout << ruta[k].getId() << ", ";
  }
  cout << endl;
  ruta = Vehicles[1].getRoute();
  for (int k = 0; k < ruta.size(); k++) {
    cout << ruta[k].getId() << ", ";
  }
}