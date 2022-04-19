#include "Grasp.h"

bool UnassignedCustomerExists(vector<Node> nodes) {
  for (int i = 1; i < nodes.size(); i++) {
    if (!nodes[i].getRouted()) return true;
  }
  return false;
}

Solution Grasp::solve(Vrp vrp) {
  Nodes = vrp.getNodesCopy();
  return ConstructGrasp(vrp);
}

Solution Grasp::ConstructGrasp(Vrp vrp) {
  vector<Node> solution;
  vector<Node> candidates;
  solution.push_back(Nodes[0]);
  Node actual = Nodes[0];
  double CandCost;
  int i = 0;
  int cost = 0;
  auto costMatrix = vrp.getDistances();
  Nodes[0].setRouted(true);
  cout << Nodes[0].getRouted();
  auto t_start = std::chrono::high_resolution_clock::now();
  while(UnassignedCustomerExists(Nodes)) {
    candidates = LRC(vrp,actual);
    i = actual.getId();
    int random = rand() % candidates.size(); //number between 0 and c.size
    actual = candidates[random];//select random node form candidates
    solution.push_back(actual); // push selected candidate to solution
    Nodes[actual.getId()].setRouted(true);
    CandCost = costMatrix[i][actual.getId()];
    cost += CandCost;
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  auto timeCost = std::chrono::duration<double, std::milli>(t_end-t_start).count();
  Solution sol(timeCost,vrp.getNumberOfCustomers(),cost,2);
  return sol;
}
vector<Node> Grasp::LRC(Vrp vrp, Node actual, int limit) {
  int nodes = 0;
  double CandCost;
  double EndCost;
  vector<Node> auxiliar = Nodes;
  vector<Node> solution;
  auto distances = vrp.getDistances();
  while(nodes < limit && UnassignedCustomerExists(auxiliar)) {
    double minCost = (double)INT_MAX;
    int CustIndex = 0;
    Node Candidate;
    //cout << "LRC";
    for (int i = 0; i < vrp.getNumberOfCustomers(); i++) {
      if (auxiliar[i].getRouted() == false) {
        CandCost = distances[actual.getId()][i];
        if (minCost > CandCost) {
          minCost = CandCost;
          CustIndex = i;
          Candidate = Nodes[i];
          nodes++;
        }
      }
    }
    auxiliar[CustIndex].setRouted(true);
    solution.push_back(Candidate);
  }
  return solution;
}