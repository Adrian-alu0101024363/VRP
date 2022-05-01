#include "../headers/Grasp.h"

bool UnassignedCustomerExists(vector<Node> nodes) {
  for (int i = 1; i < nodes.size(); i++) {
    if (!nodes[i].getRouted()) return true;
  }
  return false;
}

Solution Grasp::solve(Vrp vrp, int rlc, string method) {
  Nodes = vrp.getNodesCopy();
  Solution temp,final_solution;
  auto t_start = std::chrono::high_resolution_clock::now();
  temp = ConstructGrasp(vrp, rlc);
  final_solution = LocalSearch(temp, vrp, method);
  auto t_end = std::chrono::high_resolution_clock::now();
  auto timeCost = std::chrono::duration<double, std::milli>(t_end-t_start).count();
  final_solution.setTimeCost(timeCost);
  return final_solution;
}

Solution Grasp::ConstructGrasp(Vrp vrp, int rlc) {
  vector<Node> solution;
  vector<Node> candidates;
  solution.push_back(Nodes[0]);
  Node actual = Nodes[0];
  double CandCost, EndCost;
  int i = 0;
  int cost = 0;
  int VehIndex = 0;
  vector<Vehicle> Vehicles = vrp.getVehices();
  auto costMatrix = vrp.getDistances();
  Nodes[0].setRouted(true);
  //cout << Nodes[0].getRouted();
  int NoOfCustomers = vrp.getNumberOfCustomers();
  int NoOfVehicles = vrp.getNumberOfVehicles();
  for (int j = 0; j < vrp.getNumberOfVehicles(); j++) {
    Vehicles[j].setCurrent(0);
    Vehicles[j].AddNode(Nodes[0]);
  }
  int limit = (NoOfCustomers / NoOfVehicles) + (NoOfCustomers * 0.1);
  int cont = 0;

  while(UnassignedCustomerExists(Nodes)) {
    if (cont == limit) {
      Vehicles[VehIndex].AddNode(Nodes[0]);
      EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
      cost += EndCost;
      VehIndex++;
      Vehicles[VehIndex].AddNode(Nodes[0]);
      cont = 0;
    }
    candidates = LRC(vrp,actual,rlc);
    i = actual.getId();
    int random = rand() % candidates.size(); //number between 0 and c.size
    actual = candidates[random];//select random node form candidates
    Vehicles[VehIndex].AddNode(actual); // push selected candidate to solution
    Vehicles[VehIndex].setCurrent(Nodes[actual.getId()].getId());
    Nodes[actual.getId()].setRouted(true);
    CandCost = costMatrix[i][actual.getId()];
    cost += CandCost;
    cont++;
  }
  EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
  Vehicles[VehIndex].AddNode(Nodes[0]);
  cost += EndCost;
  Solution sol(Vehicles,0,vrp.getNumberOfCustomers(),cost,2);
  return sol;
}

vector<Node> Grasp::LRC(Vrp vrp, Node actual, int limit) {
  int nodes = 0;
  double CandCost;
  double EndCost;
  vector<Node> auxiliar = Nodes;
  vector<Node> solution;
  auto distances = vrp.getDistances();
  while(nodes < limit && UnassignedCustomerExists(auxiliar)) {
    double minCost = (double)INT_MAX;
    int CustIndex = 0;
    Node Candidate;
    //cout << "LRC";
    for (int i = 0; i < vrp.getNumberOfCustomers(); i++) {
      if (auxiliar[i].getRouted() == false) {
        CandCost = distances[actual.getId()][i];
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

Solution Grasp::LocalSearch(Solution sol, Vrp vrp, string method) {
  Solution actual = sol;
  Solution best = actual;
  /*if (method == "Intra") actual = IntraRouteLocalSearch(actual, vrp, actual.getCost());
  if (method == "Intra-swap") actual = IntraRouteLocalSearchSwap(actual, vrp, actual.getCost());
  if (method == "Inter") actual = InterRouteLocalSearch(actual, vrp, actual.getCost());
  if (method == "Inter-swap") actual = InterRouteLocalSearchSwap(actual, vrp, actual.getCost());*/
  actual = IntraRouteLocalSearchSwap(actual,vrp,actual.getCost());
  //cout << "cost: " << actual.getCost() << endl;
  if ((actual.getCost() < best.getCost())&& actual.getCost() > 0) {
    best = actual;
    //best = InterRouteLocalSearchSwap(best, vrp, actual.getCost());
    //return LocalSearch(best,vrp,method);
  }
  return best;
}

//Insertion
Solution Grasp::IntraRouteLocalSearch(Solution old, Vrp vrp, double Cost) {
  vector<Node> rt;
  double BestNCost,NeigthboorCost;
  int SwapIndexA = -1, SwapIndexB = -1, SwapRoute =-1;
  int iteration_number= 0;
  bool Termination = false;
  auto Vehicles = old.getRoutes();
  auto CostMatrix = vrp.getDistances();
  BestNCost = INT_MAX;
  for (int VehIndex = 0; VehIndex < Vehicles.size(); VehIndex++) {
    rt = Vehicles[VehIndex].getRoute();
    int RoutLength = rt.size();
    for (int i = 1; i < rt.size() - 1; i++) { //Not possible to move depot!
      for (int j = 0; j < rt.size()-1; j++) {//Not possible to move after last Depot!
        if ( ( j != i ) && (j != i-1) ) { // Not a move that changes solution cost

          double MinusCost1 = CostMatrix[rt[i-1].getId()][rt[i].getId()];
          double MinusCost2 =  CostMatrix[rt[i].getId()][rt[i+1].getId()];
          double MinusCost3 =  CostMatrix[rt[j].getId()][rt[j+1].getId()];

          double AddedCost1 = CostMatrix[rt[i-1].getId()][rt[i+1].getId()];
          double AddedCost2 = CostMatrix[rt[j].getId()][rt[i].getId()];
          double AddedCost3 = CostMatrix[rt[i].getId()][rt[j+1].getId()];

          NeigthboorCost = AddedCost1 + AddedCost2 + AddedCost3
                  - MinusCost1 - MinusCost2 - MinusCost3;
          
          if (NeigthboorCost < BestNCost) {
            BestNCost = NeigthboorCost;
            SwapIndexA  = i;
            SwapIndexB  = j;
            SwapRoute = VehIndex;
          }
        }
      }
    }
  }
  if (BestNCost < 0) {
    rt = Vehicles[SwapRoute].getRoute();
    Node SwapNode = rt[SwapIndexA];
    rt.erase(next(rt.begin(),SwapIndexA));
    if (SwapIndexA < SwapIndexB) { 
      rt.insert(next(rt.begin(),SwapIndexB), SwapNode); 
    } else { 
      rt.insert(next(rt.begin(),SwapIndexB+1), SwapNode); 
    }
    Cost  += BestNCost;
    //std::cout << "best local cost: " << Cost << endl;
    double temp1 = Vehicles[0].getRouteCost(vrp.getDistances());
    double temp2 = Vehicles[1].getRouteCost(vrp.getDistances());
    cout << "Coste total: " << temp1 + temp2;
    Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
    return sol;
  }
  Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
  return sol;
}

Solution Grasp::IntraRouteLocalSearchSwap(Solution old, Vrp vrp, double Cost) {
  vector<Node> rt;
  double BestNCost,NeigthboorCost;
  int SwapIndexA = -1, SwapIndexB = -1, SwapRoute =-1;
  int iteration_number= 0;
  auto Vehicles = old.getRoutes();
  auto CostMatrix = vrp.getDistances();
  BestNCost = INT_MAX;
  iteration_number++;
  for (int VehIndex = 0; VehIndex < Vehicles.size(); VehIndex++) {
    rt = Vehicles[VehIndex].getRoute();
    int RoutLength = rt.size();
    for (int i = 1; i < rt.size() - 1; i++) { //Not possible to move depot!
      for (int j = 0; j < rt.size()-1; j++) {//Not possible to move after last Depot!
        if ( ( j != i ) && (j != i-1) ) { // Not a move that cHanges solution cost

          double MinusCost1 = CostMatrix[rt[i-1].getId()][rt[i].getId()];
          double MinusCost2 =  CostMatrix[rt[i].getId()][rt[i+1].getId()];
          double MinusCost3 =  CostMatrix[rt[j].getId()][rt[j+1].getId()];

          double AddedCost1 = CostMatrix[rt[i-1].getId()][rt[i+1].getId()];
          double AddedCost2 = CostMatrix[rt[j].getId()][rt[i].getId()];
          double AddedCost3 = CostMatrix[rt[i].getId()][rt[j+1].getId()];

          NeigthboorCost = AddedCost1 + AddedCost2 + AddedCost3
                  - MinusCost1 - MinusCost2 - MinusCost3;
          
          if (NeigthboorCost < BestNCost) {
            BestNCost = NeigthboorCost;
            SwapIndexA  = i;
            SwapIndexB  = j;
            SwapRoute = VehIndex;
          }
        }
      }
    }
    }
    if (BestNCost < 0) {
      rt = Vehicles[SwapRoute].getRoute();
      swap(rt[SwapIndexA], rt[SwapIndexB]);
      Cost  += BestNCost;
      Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
      sol.checkCost(vrp.getDistances());
      sol.PrintNodes();
      return sol;
    }
  Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
  return sol;
}

Solution Grasp::InterRouteLocalSearch(Solution old, Vrp vrp, double Cost) {
  vector<Node> RouteFrom;
  vector<Node> RouteTo;
  int MovingNodeDemand = 0;
  int VehIndexFrom,VehIndexTo;
  double BestNCost,NeigthboorCost;
  int SwapIndexA = -1, SwapIndexB = -1, SwapRouteFrom =-1, SwapRouteTo=-1;
  int MAX_ITERATIONS = 10000;
  int iteration_number= 0;
  auto Vehicles = old.getRoutes();
  auto CostMatrix = vrp.getDistances();
  BestNCost = INT_MAX;
  for (VehIndexFrom = 0;  VehIndexFrom < Vehicles.size();  VehIndexFrom++) {
      RouteFrom = Vehicles[VehIndexFrom].getRoute();
      int RoutFromLength = RouteFrom.size();
      for (int i = 1; i < RoutFromLength - 1; i++) { //Not possible to move depot!
          for (VehIndexTo = 0; VehIndexTo < Vehicles.size(); VehIndexTo++) {
            RouteTo =  Vehicles[VehIndexTo].getRoute();
            int RouteTolength = RouteTo.size();
            for (int j = 0; (j < RouteTolength - 1); j++) {//Not possible to move after last Depot!
              if (( (VehIndexFrom == VehIndexTo) && ((j == i) || (j == i - 1)) ) == false)  // Not a move that Changes solution cost
              {
                double MinusCost1 = CostMatrix[RouteFrom[i - 1].getId()][RouteFrom[i].getId()];
                double MinusCost2 = CostMatrix[RouteFrom[i].getId()][RouteFrom[i + 1].getId()];
                double MinusCost3 = CostMatrix[RouteTo[j].getId()][RouteTo[j + 1].getId()];

                double AddedCost1 = CostMatrix[RouteFrom[i - 1].getId()][RouteFrom[i + 1].getId()];
                double AddedCost2 = CostMatrix[RouteTo[j].getId()][RouteFrom[i].getId()];
                double AddedCost3 = CostMatrix[RouteFrom[i].getId()][RouteTo[j + 1].getId()];

                NeigthboorCost = AddedCost1 + AddedCost2 + AddedCost3
                        - MinusCost1 - MinusCost2 - MinusCost3;

                if (NeigthboorCost < BestNCost) {
                  BestNCost = NeigthboorCost;
                  SwapIndexA = i;
                  SwapIndexB = j;
                  SwapRouteFrom = VehIndexFrom;
                  SwapRouteTo = VehIndexTo;
                }
              }
            }
          }
      }
  }

    if (BestNCost < 0) {// If Best Neightboor Cost is better than the current
        RouteFrom = Vehicles[SwapRouteFrom].getRoute();
        RouteTo = Vehicles[SwapRouteTo].getRoute();
        Node SwapNode = RouteFrom[SwapIndexA];
        RouteFrom.erase(next(RouteFrom.begin(), SwapIndexA));
        if (SwapRouteFrom == SwapRouteTo) {
            if (SwapIndexA < SwapIndexB) {
              RouteTo.insert(next(RouteTo.begin(),SwapIndexB), SwapNode);
            } else {
              RouteTo.insert(next(RouteTo.begin(),SwapIndexB + 1), SwapNode);
            }
        } else {
          RouteTo.insert(next(RouteTo.begin(),SwapIndexB + 1), SwapNode);
        }
        Vehicles[SwapRouteFrom].setRoute(RouteFrom);
        Vehicles[SwapRouteTo].setRoute(RouteTo);
        Cost  += BestNCost;
        Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
        return sol;
    } 
  Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
  return sol;
}

Solution Grasp::InterRouteLocalSearchSwap(Solution old, Vrp vrp, double Cost) {
  vector<Node> RouteFrom;
  vector<Node> RouteTo;
  int MovingNodeDemand = 0;
  int VehIndexFrom,VehIndexTo;
  double BestNCost,NeigthboorCost;
  int SwapIndexA = -1, SwapIndexB = -1, SwapRouteFrom =-1, SwapRouteTo=-1;
  bool Termination = false;
  auto Vehicles = old.getRoutes();
  auto CostMatrix = vrp.getDistances();
  BestNCost = INT_MAX;
  for (VehIndexFrom = 0;  VehIndexFrom < Vehicles.size();  VehIndexFrom++) {
      RouteFrom = Vehicles[VehIndexFrom].getRoute();
      int RoutFromLength = RouteFrom.size();
      for (int i = 1; i < RoutFromLength - 1; i++) { //Not possible to move depot!
          for (VehIndexTo = 0; VehIndexTo < Vehicles.size(); VehIndexTo++) {
            RouteTo =  Vehicles[VehIndexTo].getRoute();
            int RouteTolength = RouteTo.size();
            for (int j = 0; (j < RouteTolength - 1); j++) {//Not possible to move after last Depot!
              if (( (VehIndexFrom == VehIndexTo) && ((j == i) || (j == i - 1)) ) == false)  // Not a move that Changes solution cost
              {
                double MinusCost1 = CostMatrix[RouteFrom[i - 1].getId()][RouteFrom[i].getId()];
                double MinusCost2 = CostMatrix[RouteFrom[i].getId()][RouteFrom[i + 1].getId()];
                double MinusCost3 = CostMatrix[RouteTo[j].getId()][RouteTo[j + 1].getId()];

                double AddedCost1 = CostMatrix[RouteFrom[i - 1].getId()][RouteFrom[i + 1].getId()];
                double AddedCost2 = CostMatrix[RouteTo[j].getId()][RouteFrom[i].getId()];
                double AddedCost3 = CostMatrix[RouteFrom[i].getId()][RouteTo[j + 1].getId()];

                NeigthboorCost = AddedCost1 + AddedCost2 + AddedCost3
                        - MinusCost1 - MinusCost2 - MinusCost3;

                if (NeigthboorCost < BestNCost) {
                  BestNCost = NeigthboorCost;
                  SwapIndexA = i;
                  SwapIndexB = j;
                  SwapRouteFrom = VehIndexFrom;
                  SwapRouteTo = VehIndexTo;
                }
              }
            }
          }
      }
  }
    if (BestNCost < 0) {// If Best Neightboor Cost is better than the current
        RouteFrom = Vehicles[SwapRouteFrom].getRoute();
        RouteTo = Vehicles[SwapRouteTo].getRoute();
        Node SwapNode2 = RouteTo[SwapIndexB];
        Node SwapNode = RouteFrom[SwapIndexA];
        RouteFrom[SwapIndexA] = SwapNode2;
        RouteTo[SwapIndexB] = SwapNode;
        Vehicles[SwapRouteFrom].setRoute(RouteFrom);
        Vehicles[SwapRouteTo].setRoute(RouteTo);
        Cost  += BestNCost;
        Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
        return sol;
    } 
  Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
  return sol;
}