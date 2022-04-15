#include "Vehicle.h"

using namespace std;

Vehicle::Vehicle(int id, int capacity) {
  VehId = id;
  capacity_ = capacity;
  load_ = 0;
  Route = new vector<Node>;
}

void Vehicle::AddNode(Node customer) {
  Route->push_back(customer);
  load_ += customer.getDemand();
}

bool Vehicle::CheckIfFits(int demand) {
  return ((load_ + demand <= capacity_));
}