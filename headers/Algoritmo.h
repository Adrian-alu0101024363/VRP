#ifndef TRAVERSE
#define TRAVERSE
#include "Solution.h"
#include <string>

using namespace std;

class Vrp;

class Traverse {
public:
virtual Solution solve(Vrp vrp, int rlc, string method = "Inter-swap") = 0;
}; 


#endif