#include "VRP.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Please specify the path to file" << endl;
  } else {
    string path = argv[1];
    VRP vrp(path);
    vrp.print();
    vrp.GreedySolution();
  }
}