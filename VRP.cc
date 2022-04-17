#include "VRP.h"

using namespace std;

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' ');}

/**
 * @brief read the file and store it as a vrp
 * 
 * @param file the file with the specification of a vrp
 */
void VRP::read(string file) {
  ifstream f;
  f.open(file);
  string line;
  f >> line;
  f >> NoOfCustomers;
  NoOfCustomers++;
  f >> line;
  f >> NoOfVehicles;
  Vehicles.resize(NoOfVehicles);
  getline(f, line);
  getline(f, line);
  int i = 0;
  for (size_t i{0}; i != NoOfCustomers; ++i)
  {
    costMatrix.emplace_back(NoOfCustomers);
  }
  while (getline(f, line)) {
    string str = line;
    std::replace(std::begin(str),std::end(str),'\t',' ');
    std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
    str.erase(new_end, str.end());
    vector<double> val = values(str);
    Node node(i,0);
    Nodes.push_back(node);
    for (int j = 0; j < val.size(); j++) {
      //cout << val[j];
      costMatrix[i][j] = val[j];
    }
    i++;
  }
  Nodes[0].setDepot(true);
}

/**
 * @brief from a line of text to a vector of double
 * used to store the distances to the nodes
 * @param line the line read from the file
 * @return std::vector<double> the distance values
 */
std::vector<double> VRP::values(std::string line) {
  char space_char = ' ';
  vector<double> val;
  stringstream sstream(line);
  string number;
  while (getline(sstream, number, space_char)){
    val.push_back(stod(number));
  }
  return val;
}

/**
 * @brief rough print to console the vrp
 * 
 */
void VRP::print() {
  cout << "Number of vehicles: " << NoOfVehicles << endl;
  cout << "Number of clients: " << NoOfCustomers << endl;
  cout << "Sad? " << UnassignedCustomerExists(Nodes);
  for (int i = 0; i < costMatrix.size(); i++) {
    for (int j = 0; j < costMatrix.size(); j++) {
      cout << costMatrix[i][j];
    }
    cout << endl;
  }
}

/**
 * @brief check if there is a non visited node
 * 
 * @param nodes the list of nodes to check
 * @return true 
 * @return false 
 */
bool VRP::UnassignedCustomerExists(vector<Node> nodes) {
  for (int i = 1; i < nodes.size(); i++) {
    if (!nodes[i].getRouted()) return true;
  }
  return false;
}

/**
 * @brief Greedy not optimal solution to the vrp problem
 * 
 */
void VRP::GreedySolution() {
  double CandCost, EndCost;
  int VehIndex = 0;
  for (int j = 0; j < NoOfVehicles; j++) {
    Vehicles[j].setCurrent(0);
  }
  int limit = (NoOfCustomers / NoOfVehicles) + 1; //cuidado con inpar
  int cont = 0;
  auto t_start = std::chrono::high_resolution_clock::now();
  while (UnassignedCustomerExists(Nodes)) {
    int CustIndex = 0;
    Node Candidate;
    double minCost = (double)INT_MAX;
    /*if (Vehicles[VehIndex].getRoute().size() == 0) {
        //Vehicles[VehIndex].AddNode(Nodes[0]);
    }*/
    if (cont == limit) {
      Vehicles[VehIndex].AddNode(Nodes[0]);
      EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
      cost += EndCost;
      VehIndex++;
      Vehicles[VehIndex].AddNode(Nodes[0]);
      cont = 0;
    }
    for (int i = 0; i < NoOfCustomers; i++) {
      if (Nodes[i].getRouted() == false) {
        int pos = Vehicles[VehIndex].getCurrent();
        CandCost = costMatrix[pos][i];
        //if (Vehicles[VehIndex].CheckIfFits(Nodes[i].getDemand())) {
        //if (Vehicles[VehIndex].CheckIfFits(CandCost)) {
        if (minCost > CandCost) {
          //cout << "En: " << pos + 1<< " : " << i + 1 << endl;
          //cout << CandCost << endl;
          minCost = CandCost;
          CustIndex = i;
          Candidate = Nodes[i];
        }
        //}
      }
    }
    //cout << "in node: " << Nodes[CustIndex].getId() +1 << endl;
    Vehicles[VehIndex].AddNode(Candidate);
    Nodes[CustIndex].setRouted(true);
    Vehicles[VehIndex].setCurrent(Nodes[CustIndex].getId());
    cost += minCost;
    cont++;
  }
  EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
  Vehicles[VehIndex].AddNode(Nodes[0]);
  cost += EndCost;
  auto t_end = std::chrono::high_resolution_clock::now();
  timeCost = std::chrono::duration<double, std::milli>(t_end-t_start).count();
  /*cout << "Coste final: " << cost << endl;
  cout << "Ruta:" << endl;
  auto ruta = Vehicles[0].getRoute();
  for (int k = 0; k < ruta.size(); k++) {
    cout << ruta[k].getId() << ", ";
  }
  cout << endl;
  ruta = Vehicles[1].getRoute();
  for (int k = 0; k < ruta.size(); k++) {
    cout << ruta[k].getId() << ", ";
  }*/
}

/**
 * @brief print to console the greedy solution information
 * 
 */
void VRP::printGreedyTableu() {
  cout << name << setw(4) << NoOfCustomers << setw(8) << cost;
  cout << setw(10) << timeCost << endl;
}

/**
 * @brief LRC that choose the 4 best elements greedy based
 * 
 * @param actual the node that we want to evaluate
 * @return vector<Node> the candidates
 */
vector<Node> VRP::LRC(Node actual) {
  int nodes = 0;
  double CandCost;
  vector<Node> auxiliar = Nodes;
  vector<Node> solution;
  while(nodes < 4 && UnassignedCustomerExists(auxiliar)) {
    double minCost = (double)INT_MAX;
    int CustIndex = 0;
    Node Candidate;
    cout << "LRC";
    for (int i = 0; i < NoOfCustomers; i++) {
      if (auxiliar[i].getRouted() == false) {
        CandCost = costMatrix[actual.getId()][i];
        if (minCost > CandCost) {
          minCost = CandCost;
          CustIndex = i;
          Candidate = Nodes[i];
          nodes++;
        }
      }
    }
    auxiliar[CustIndex].setRouted(true);
    solution.push_back(Candidate);
  }
  return solution;
}

/**
 * @brief Construct phase of the grasp algorithm
 * choose a random node from a list of candidates rlc
 * @return vector<Node> 
 */
vector<Node> VRP::ConstructGrasp() {
  vector<Node> solution;
  vector<Node> candidates;
  solution.push_back(Nodes[0]);
  Node actual = Nodes[0];
  Nodes[0].setRouted(true);
  while(UnassignedCustomerExists(Nodes)) {
    candidates = LRC(actual);
    int random = rand() % candidates.size(); //number between 0 and c.size
    actual = candidates[random];//select random node form candidates
    solution.push_back(actual); // push selected candidate to solution
    Nodes[actual.getId()].setRouted(true);
    //adapt greedy function(candidates)?
  }
  return solution;
}

/*
procedure ConstructGreedyRandomizedSolution(Solution)
1 Solution = {};
2 for Solution construction not done →
3 MakeRCL(RCL);
4 s = SelectElementAtRandom(RCL);
5 Solution = Solution∪{s};
6 AdaptGreedyFunction(s);
7 rof;
end ConstructGreedyRandomizedSolution;
*/