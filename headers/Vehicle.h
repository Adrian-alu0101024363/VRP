#ifndef VEHICLE
#define VEHICLE
#include "Node.h"
#include <vector>
#include <iostream>

using namespace std;

class Vehicle {
 private:
  int VehId;
  std::vector<Node> Route;
  int capacity_;
  int load_;
  int current_;
 public:
  Vehicle(){}
  Vehicle(int id, int capacity);
  ~Vehicle(){}
  int getVehId() {return VehId;}
  int getCapacity() {return capacity_;}
  int getVehId() const {return VehId;}
  int getCapacity() const {return capacity_;}
  int getLoad() {return load_;}
  int getLoad() const {return load_;}
  int getCurrent() const {return current_;}
  int getCurrent() {return current_;}
  void setCurrent(int current) {current_ = current;}
  void setVehId(int id) {VehId = id;}
  void setCapacity(int capacity) {capacity_ = capacity;}
  void setLoad(int load) {load_ = load;}
  std::vector<Node> getRoute() {return Route;}
  void setRoute(std::vector<Node> route) {Route = route;}
  void AddNode(Node customer);
  bool CheckIfFits(int demand);
  double getRouteCost(vector<vector<double>> costMatrix);
  void print();
};

#endif