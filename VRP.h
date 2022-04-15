#include "Node.h"
#include "Vehicle.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

using std::vector;
using std::string;

class VRP {

public:
  int NoOfVehicles;
  int NoOfCustomers;
  std::vector<Vehicle> Vehicles;
  double cost;
  std::vector<Node> Nodes;
  std::vector<std::vector<double>> costMatrix;
  VRP(std::string file) {read(file);}
  ~VRP(){}
  void read(std::string file);
  void print();
  std::vector<double> values(std::string line);
  bool UnassignedCustomerExists()
};