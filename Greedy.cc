#include "Greedy.h"

Solution Greedy::solve(Vrp vrp) {
  double CandCost, EndCost;
  int VehIndex = 0;
  double cost = 0;
  vector<Vehicle> Vehicles = vrp.getVehices();
  vector<Node> Nodes = vrp.getNodes();
  vector<vector<double>> costMatrix = vrp.getDistances();
  int NoOfCustomers = vrp.getNumberOfCustomers();
  int NoOfVehicles = vrp.getNumberOfVehicles();
  for (int j = 0; j < vrp.getNumberOfVehicles(); j++) {
    Vehicles[j].setCurrent(0);
  }
  int limit = (NoOfCustomers / NoOfVehicles) + 1; //cuidado con inpar
  int cont = 0;
  auto t_start = std::chrono::high_resolution_clock::now();
  while (vrp.UnassignedCustomerExists(Nodes)) {
    int CustIndex = 0;
    Node Candidate;
    double minCost = (double)INT_MAX;
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
        if (minCost > CandCost) {
          minCost = CandCost;
          CustIndex = i;
          Candidate = Nodes[i];
        }
      }
    }
    Vehicles[VehIndex].AddNode(Candidate);
    Nodes[CustIndex].setRouted(true);
    Vehicles[VehIndex].setCurrent(Nodes[CustIndex].getId());
    cost += minCost;
    cont++;
  }
  EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
  Vehicles[VehIndex].AddNode(Nodes[0]);
  cost += EndCost;
  auto t_end = std::chrono::high_resolution_clock::now();
  auto timeCost = std::chrono::duration<double, std::milli>(t_end-t_start).count();
  Solution sol(timeCost,vrp.getNumberOfCustomers(),cost);
  return sol;
}