#include "Algoritmo.h"
#include "VRP.h"

using namespace std;

class Gvns : public Traverse {
  public:
  vector<Node> Nodes;
  Solution solve(Vrp vrp, int rlc, int method = 1);
  Solution grasp(Vrp vrp, int rlc, int method = 1);
  Solution rvns(Vrp vrp, Solution sol);
  Solution shake(Vrp vrp, Solution sol, int i);
  Solution ConstructGrasp(Vrp vrp, int rlc);
  vector<Node> LRC(Vrp vrp, Node actual, int limit = 2);
  Solution LocalSearch(Solution sol, Vrp vrp, int method = 1);
  Solution IntraRouteLocalSearch(Solution old, Vrp vrp, double Cost);
  Solution IntraRouteLocalSearchSwap(Solution old, Vrp vrp, double Cost);
  Solution InterRouteLocalSearch(Solution old, Vrp vrp, double Cost);
  Solution InterRouteLocalSearchSwap(Solution old, Vrp vrp, double Cost);
  bool UnassignedCustomerExists(vector<Node> nodes);
  Solution RandomSearch(Vrp vrp, Solution sol, int i);
};