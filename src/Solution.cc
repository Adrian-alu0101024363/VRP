#include "../headers/Solution.h"

/**
 * @brief Construct a new Solution:: Solution object
 * Consist of the time consumed the number of clients, 
 * the cost of the vrp and the rcl used
 * @param time 
 * @param size 
 * @param cost 
 * @param RCL 
 */
Solution::Solution(double time, int size, double cost, int RCL) {
  timeCost = time;
  cost_ = cost;
  size_ = size;
}

/**
 * @brief Construct a new Solution:: Solution object
 * Consist of a vector of vehicles the time consumed 
 * the number of clients, the cost of the vrp and the rcl used
 * @param routes 
 * @param time 
 * @param size 
 * @param cost 
 * @param RCL 
 */
Solution::Solution(vector<Vehicle> routes, double time, int size, double cost, int RCL) {
  Routes = routes;
  timeCost = time;
  cost_ = cost;
  size_ = size;
}

/**
 * @brief Check if the the solution cost is correct for
 * the given cost matrix
 * @param costMatrix 
 */
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

/**
 * @brief Print to console the nodes of the solution
 * 
 */
void Solution::PrintNodes() {
  for (int i = 0; i < Routes.size(); i++) {
    auto route = Routes[i];
    route.print();
  }
}