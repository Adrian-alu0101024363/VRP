#include "../headers/VRP.h"

using namespace std;

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' ');}

/**
 * @brief read the file and store it as a Vrp
 * 
 * @param file the file with the specification of a Vrp
 */
void Vrp::read(string file) {
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
std::vector<double> Vrp::values(std::string line) {
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
 * @brief rough print to console the Vrp
 * 
 */
void Vrp::print() {
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
bool Vrp::UnassignedCustomerExists(vector<Node> nodes) {
  for (int i = 1; i < nodes.size(); i++) {
    if (!nodes[i].getRouted()) return true;
  }
  return false;
}

/**
 * @brief print to console the greedy solution information
 * 
 */
void Vrp::printGreedyTableu() {
  cout << name << setw(4) << NoOfCustomers << setw(8) << cost;
  cout << setw(10) << timeCost << endl;
}

/**
 * @brief 
 * 
 */
void Vrp::printGraspTableu() {
  cout << name << setw(4) << NoOfCustomers << setw(8) << cost;
  cout << setw(14) << timeCost << setw(4) << candidatesNumber << endl;
}

/**
 * @brief LRC that choose the 4 best elements greedy based
 * 
 * @param actual the node that we want to evaluate
 * @return vector<Node> the candidates
 */
vector<Node> Vrp::LRC(Node actual, int limit) {
  int nodes = 0;
  double CandCost;
  double EndCost;
  candidatesNumber = limit;
  vector<Node> auxiliar = Nodes;
  vector<Node> solution;
  while(nodes < limit && UnassignedCustomerExists(auxiliar)) {
    double minCost = (double)INT_MAX;
    int CustIndex = 0;
    Node Candidate;
    //cout << "LRC";
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
vector<Node> Vrp::ConstructGrasp() {
  vector<Node> solution;
  vector<Node> candidates;
  solution.push_back(Nodes[0]);
  Node actual = Nodes[0];
  double CandCost;
  int i = 0;
  cost = 0;
  Nodes[0].setRouted(true);
  while(UnassignedCustomerExists(Nodes)) {
    candidates = LRC(actual);
    i = actual.getId();
    int random = rand() % candidates.size(); //number between 0 and c.size
    actual = candidates[random];//select random node form candidates
    solution.push_back(actual); // push selected candidate to solution
    Nodes[actual.getId()].setRouted(true);
    CandCost = costMatrix[i][actual.getId()];
    cost += CandCost;
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