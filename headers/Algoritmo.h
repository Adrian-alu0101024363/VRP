#ifndef TRAVERSE
#define TRAVERSE
#include "Solution.h"

using namespace std;

class Vrp;

class Traverse {
public:
virtual Solution solve(Vrp vrp) = 0;
}; 


#endif