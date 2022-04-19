#include "Algoritmo.h"
#include "VRP.h"

using namespace std;

class Grasp : public Traverse {
  public:
  vector<Node> Nodes;
  Solution solve(Vrp vrp);
  Solution ConstructGrasp(Vrp vrp);
  vector<Node> LRC(Vrp vrp, Node actual, int limit = 2);
};