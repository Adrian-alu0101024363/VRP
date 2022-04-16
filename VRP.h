#include "Node.h"
#include "Vehicle.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <climits>
#include <regex>

using std::vector;
using std::string;

class VRP {

public:
  int NoOfVehicles;
  int NoOfCustomers;
  vector<Vehicle> Vehicles;
  double cost;
  vector<Node> Nodes;
  vector<std::vector<double>> costMatrix;
  VRP(std::string file) {read(file);}
  ~VRP(){}
  void read(string file);
  void print();
  vector<double> values(string line);
  bool UnassignedCustomerExists(vector<Node> nodes);
  void GreedySolution();
};