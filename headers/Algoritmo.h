#ifndef TRAVERSE
#define TRAVERSE
#include "Solution.h"
#include <string>

using namespace std;

class Vrp;

class Traverse {
public:
virtual Solution solve(Vrp vrp, int rlc, int method = 1) = 0;
}; 


#endif