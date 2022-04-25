#include "Algoritmo.h"
#include "VRP.h"

using namespace std;

class Greedy : public Traverse {
  public:
  Solution solve(Vrp vrp, int rlc, string method = "Inter-swap");
};

