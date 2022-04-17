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
#include <iomanip>
#include <chrono>

using std::vector;
using std::string;

class VRP {
private:
  int NoOfVehicles;
  int NoOfCustomers;
  vector<Vehicle> Vehicles;
  double cost;
  vector<Node> Nodes;
  vector<vector<double>> costMatrix;
  string name;
  double timeCost;
public:
  VRP(string file) {read(file);name = file;}
  ~VRP(){}
  void read(string file);
  void print();
  vector<double> values(string line);
  bool UnassignedCustomerExists(vector<Node> nodes);
  void GreedySolution();
  void printGreedyTableu();
  vector<Node> ConstructGrasp();
  vector<Node> LRC(Node actual);
};