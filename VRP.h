#ifndef VRP
#define VRP
#include "Algoritmo.h"
#include "Node.h"
#include "Vehicle.h"
#include "Solution.h"
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

using namespace std;

class Vrp {
private:
  int NoOfVehicles;
  int NoOfCustomers;
  vector<Vehicle> Vehicles;
  double cost;
  vector<Node> Nodes;
  vector<vector<double>> costMatrix;
  string name;
  double timeCost;
  int candidatesNumber;
  Traverse* method_;
public:
  Vrp(string file, Traverse* met) {read(file);name = file; method_ = met;}
  ~Vrp(){}
  void read(string file);
  void print();
  int getNumberOfVehicles() {return NoOfVehicles;}
  int getNumberOfCustomers() {return NoOfCustomers;}
  vector<Vehicle>& getVehices() {return Vehicles;}
  vector<Node>& getNodes() {return Nodes;}
  vector<Node> getNodesCopy() {return Nodes;}
  vector<vector<double>> getDistances() {return costMatrix;}
  vector<double> values(string line);
  bool UnassignedCustomerExists(vector<Node> nodes);
  void GreedySolution();
  void printGreedyTableu();
  void printGraspTableu();
  vector<Node> ConstructGrasp();
  vector<Node> LRC(Node actual, int limit = 2);
  Solution solve(){return method_->solve(*this);}
};
#endif
// Clase solution, clase problema como en el tsp, algoritmo, grasp en un bucle.