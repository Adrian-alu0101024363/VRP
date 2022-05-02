#include "headers/VRP.h"
#include "headers/Greedy.h"
#include "headers/Grasp.h"
#include "headers/Gvns.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Please specify the path to file" << endl;
  } else {
    string path = argv[1];
    //srand((unsigned) time(0));
    /*Vrp vrp(path, new Greedy());
    Solution sol;
    sol = vrp.solve(0);
    cout << setw(8) << "Algoritmo voraz" << endl;
    cout << "Problema" << setw(6) << "n ";
    cout << setw(12) << "Distancia" << setw(6) << "Time" << endl;
    cout << path << setw(4) << vrp.getNumberOfCustomers() - 1<< setw(8) << sol.getCost();
    cout << setw(10) << sol.getTimeCost() << endl;
    */
    cout << setw(8) << "Algoritmo grasp" << endl;
    cout << "Problema" << setw(6) << "n ";
    cout << setw(12) << "Distancia" << setw(6) << "Time" << setw(6) << "RLC" << endl;
    int i = 0;
    int limit = 5;
    while (i < limit) {
      Vrp vrp2(path, new Grasp());
      auto sol2 = vrp2.solve(i + 2);
      cout << path << setw(4) << vrp2.getNumberOfCustomers() - 1<< setw(8) << sol2.getCost();
      cout << setw(12) << sol2.getTimeCost() << setw(4) << i + 2 << endl;
      i++;
    }
    i = 0;
    cout << setw(8) << "Algoritmo gvns" << endl;
    cout << "Problema" << setw(6) << "n ";
    cout << setw(12) << "Distancia" << setw(6) << "Time" << setw(6) << "RLC" << endl;
    while (i < limit) {
      Vrp vrp3(path, new Gvns());
      Solution sol3;
      sol3 = vrp3.solve(i + 2);
      cout << path << setw(4) << vrp3.getNumberOfCustomers() - 1<< setw(8) << sol3.getCost();
      cout << setw(12) << sol3.getTimeCost() << setw(4) << i + 2 << endl;
      i++;
    }
  }
}