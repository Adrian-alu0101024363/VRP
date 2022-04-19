#include "headers/VRP.h"
#include "headers/Greedy.h"
#include "headers/Grasp.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Please specify the path to file" << endl;
  } else {
    string path = argv[1];
    Vrp vrp(path, new Greedy());
    Solution sol;
    sol = vrp.solve();
    cout << setw(8) << "Algoritmo voraz" << endl;
    cout << "Problema" << setw(6) << "n ";
    cout << setw(12) << "Distancia" << setw(6) << "Time" << endl;
    cout << path << setw(4) << vrp.getNumberOfCustomers() << setw(8) << sol.getCost();
    cout << setw(10) << sol.getTimeCost() << endl;
    Vrp vrp2(path, new Grasp());
    auto sol2 = vrp2.solve();
    cout << setw(8) << "Algoritmo grasp" << endl;
    cout << "Problema" << setw(6) << "n ";
    cout << setw(12) << "Distancia" << setw(6) << "Time" << setw(6) << "RLC" << endl;
    cout << path << setw(4) << vrp2.getNumberOfCustomers() << setw(8) << sol2.getCost();
    cout << setw(14) << sol2.getTimeCost() << setw(4) << 2 << endl;
  }
}