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