#include "../headers/Vehicle.h"

using namespace std;

/**
 * @brief Construct a new Vehicle:: Vehicle object
 * 
 * @param id vehicle identificator
 * @param capacity capacity of vehicle (not used for now)
 */
Vehicle::Vehicle(int id, int capacity) {
  VehId = id;
  capacity_ = capacity;
  load_ = 0;
  //Route = new vector<Node>;
}

/**
 * @brief Add the given node to the vehicle route
 * 
 * @param customer the node to be added
 */
void Vehicle::AddNode(Node customer) {
  Route.push_back(customer);
  //load_ += customer.getDemand();
}

/*
bool Vehicle::CheckIfFits(int demand) {
  return ((load_ + demand <= capacity_));
}*/

double Vehicle::getRouteCost(vector<vector<double>> costMatrix) {
  double cost = 0;
  for (int i = 0; i < Route.size() - 1; i++) {
    cost += costMatrix[Route[i].getId()][Route[i+1].getId()];
  }
  return cost;
}

void Vehicle::print() {
  for (int i = 0; i < Route.size(); i++) {
    cout << Route[i].getId() << ",";
  }
  cout << endl;
}