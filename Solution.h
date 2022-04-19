#ifndef SOLUTION
#define SOLUTION
#include "Node.h"
#include "Solution.h"
#include <vector>

using namespace std;

class Solution {
 private:
  double timeCost;
  vector<vector<Node>> Routes;
  double cost_;
  int size_;
 public:
  Solution(){}
  Solution(double time, int size, double cost, int RCL = 0);
  int getSize() {return size_;}
  double getTimeCost() {return timeCost;}
  double getCost() {return cost_;}
};
#endif