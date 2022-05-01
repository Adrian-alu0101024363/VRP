#include "../headers/Solution.h"

Solution::Solution(double time, int size, double cost, int RCL) {
  timeCost = time;
  cost_ = cost;
  size_ = size;
}

Solution::Solution(vector<Vehicle> routes, double time, int size, double cost, int RCL) {
  Routes = routes;
  timeCost = time;
  cost_ = cost;
  size_ = size;
}

void Solution::checkCost(vector<vector<double>> costMatrix) {
  double total = 0;
  for (int i = 0; i < Routes.size(); i++) {
    auto route = Routes[i];
    double cap1 = route.getRouteCost(costMatrix);
    total += cap1;
  }
  if (total == getCost()) {
    cout << "Cost is correct" << endl;
  } else {
    cout << "Correct is: " << total << " Cost is:" << getCost() << endl;
  }

}

void Solution::PrintNodes() {
  for (int i = 0; i < Routes.size(); i++) {
    auto route = Routes[i];
    route.print();
  }
}