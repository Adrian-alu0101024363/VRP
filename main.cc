#include "VRP.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Please specify the path to file" << endl;
  } else {
    string path = argv[1];
    VRP vrp(path);
    //vector<Node> sol = vrp.ConstructGrasp();
    //cout << "Size es: " << sol.size() << endl;
    //for (int i = 0; i < sol.size(); i++) cout << sol[i].getId();
    //vrp.print();
    vrp.GreedySolution();
    cout << setw(8) << "Algoritmo voraz" << endl;
    cout << "Problema" << setw(6) << "n ";
    cout << setw(12) << "Distancia" << setw(6) << "Time" << endl;
    vrp.printGreedyTableu();
  }
}