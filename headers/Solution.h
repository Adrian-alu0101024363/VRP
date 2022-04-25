#ifndef SOLUTION
#define SOLUTION
#include "Node.h"
#include "Solution.h"
#include "Vehicle.h"
#include <vector>

using namespace std;

class Solution {
 private:
  double timeCost;
  vector<Vehicle> Routes;
  double cost_;
  int size_;
 public:
  Solution(){}
  Solution(double time, int size, double cost, int RCL = 0);
  Solution(vector<Vehicle> routes, double time, int size, double cost, int RCL = 0);
  int getSize() {return size_;}
  double getTimeCost() {return timeCost;}
  double getCost() {return cost_;}
  void setTimeCost(double time) {timeCost = time;}
  vector<Vehicle>& getRoutes() {return Routes;}
};
#endif