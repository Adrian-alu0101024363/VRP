#include "../headers/Gvns.h"

/**
 * @brief f Check if all the nodes of a given route
 * are visited
 * 
 * @param nodes 
 * @return true 
 * @return false 
 */
bool Gvns::UnassignedCustomerExists(vector<Node> nodes) {
  for (int i = 1; i < nodes.size(); i++) {
    if (!nodes[i].getRouted()) return true;
  }
  return false;
}

/**
 * @brief Given a vrp a rlc and the method aka what local search
 * to use it resolve the vrp with a gvns method
 * 
 * @param vrp 
 * @param rlc 
 * @param method 
 * @return Solution 
 */
Solution Gvns::solve(Vrp vrp, int rlc, int method) {
  Solution prov = grasp(vrp,rlc,method);
  Solution best = prov;
  int iter = 0;
  auto t_start = std::chrono::high_resolution_clock::now();
  while(iter < 6) {
    Solution first = grasp(vrp,rlc,method);
    Solution second = rvns(vrp, first);
    iter++;
    if (best.getCost() > second.getCost()) {
      best = second;
    }
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  auto timeCost = std::chrono::duration<double, std::milli>(t_end-t_start).count();
  best.setTimeCost(timeCost);
  return best;
}

/**
 * @brief rvns of the gvns that search for a rvns solution
 * 
 * @param vrp 
 * @param sol 
 * @return Solution 
 */
Solution Gvns::rvns(Vrp vrp, Solution sol) {
  Solution gvnsSol = sol;
  int nk = (vrp.getNumberOfCustomers() * 15) / 100;
  int i = 0;
  int j = 0;
  int nb = 3;
  while(i < nk) {
    Solution s = shake(vrp,gvnsSol,i);
    while(j < nb) {
      Solution local = LocalSearch(s,vrp,j);
      if (s.getCost() > local.getCost()) {
        s = local;
        j = 0;
      }
      j++;
    }
    // Si mejoramos valor volvemos a k1
    if (gvnsSol.getCost() > s.getCost()) {
      gvnsSol = s;
      i = 0;
    }
    i++;
  }
  return gvnsSol;
}

/**
 * @brief shake the initial grasp solution using a random
 * i insertions /swaps
 * @param vrp 
 * @param sol 
 * @param i 
 * @return Solution 
 */
Solution Gvns::shake(Vrp vrp, Solution sol, int i) {
  /*Solution shaked;
  Solution generated;
  int j = 0;
  if (i == 0) return InterRouteLocalSearch(sol,vrp,sol.getCost());
  if (i == 1) {
    int random = rand() % 3;
    Solution provisional = IntraRouteLocalSearchSwap(sol,vrp,sol.getCost());
    return IntraRouteLocalSearch(provisional,vrp,provisional.getCost());
  } 
  if (i == 2) {
    Solution provisional = IntraRouteLocalSearchSwap(sol,vrp,sol.getCost());
    Solution provisional2 = IntraRouteLocalSearch(provisional,vrp,provisional.getCost());
    return InterRouteLocalSearch(provisional2,vrp,provisional.getCost());
  }
  if (i == 3) {
    Solution provisional = IntraRouteLocalSearchSwap(sol,vrp,sol.getCost());
    Solution provisional2 = IntraRouteLocalSearch(provisional,vrp,provisional.getCost());
    Solution provisional3 = InterRouteLocalSearch(provisional2,vrp,provisional.getCost());
    //return InterRouteLocalSearchSwap(provisional3,vrp,provisional.getCost());
    return provisional3;
  }
  shaked = IntraRouteLocalSearch(sol, vrp, sol.getCost());
  return shaked;*/
  return RandomSearch(vrp,sol,i);
}

/**
 * @brief Given a vrp a rlc and the method aka what local search
 * to use it resolve the vrp with a grasp method
 * 
 * @param vrp 
 * @param rlc 
 * @param method 
 * @return Solution 
 */
Solution Gvns::grasp(Vrp vrp, int rlc, int method) {
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

/**
 * @brief shake the initial grasp solution using a random
 * i insertions /swaps
 * @param vrp 
 * @param sol 
 * @param i 
 * @return Solution 
 */
Solution Gvns::RandomSearch(Vrp vrp, Solution sol, int i) {
  int j = 0;
  Solution temp = sol;
  if (i == 0) return InterRouteLocalSearch(temp,vrp,temp.getCost());
  while (j < i) {
    int random = rand() % 2;
    if (random == 0) temp = IntraRouteLocalSearch(temp,vrp,temp.getCost());
    if (random == 1) temp = IntraRouteLocalSearchSwap(temp,vrp,temp.getCost());
    if (random == 2) temp = InterRouteLocalSearch(temp,vrp,temp.getCost());
    j++;
  }
  return temp;
}

/**
 * @brief Constructive part of grasp using rlc 
 * with a value given
 * @param vrp 
 * @param rlc 
 * @return Solution 
 */
Solution Gvns::ConstructGrasp(Vrp vrp, int rlc) {
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
      //Vehicles[VehIndex].AddNode(Nodes[0]);
      cont = 0;
      actual = Nodes[0];
    } else {
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
  }
  EndCost = costMatrix[Vehicles[VehIndex].getCurrent()][0];
  Vehicles[VehIndex].AddNode(Nodes[0]);
  cost += EndCost;
  Solution sol(Vehicles,0,vrp.getNumberOfCustomers(),cost,2);
  return sol;
}

/**
 * @brief Return a list of the best candidates within a
 * limited number limit
 * @param vrp 
 * @param actual 
 * @param limit 
 * @return vector<Node> 
 */
vector<Node> Gvns::LRC(Vrp vrp, Node actual, int limit) {
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

/**
 * @brief Recursive localSearch for grasp using the method
 * selected by the int method 
 * @param sol 
 * @param vrp 
 * @param method 
 * @return Solution 
 */
Solution Gvns::LocalSearch(Solution sol, Vrp vrp, int method) {
  Solution actual = sol;
  Solution best = actual;
  if (method == 1) actual = IntraRouteLocalSearch(actual, vrp, actual.getCost());
  if (method == 2) actual = IntraRouteLocalSearchSwap(actual, vrp, actual.getCost());
  if (method == 0) return InterRouteLocalSearch(actual, vrp, actual.getCost());
  if (method == 3) return InterRouteLocalSearchSwap(actual, vrp, actual.getCost());
  //cout << "Original disposition: ";
  //sol.PrintNodes();
  //sol.checkCost(vrp.getDistances());
  //cout << endl;
  //actual = InterRouteLocalSearchSwap(actual,vrp,sol.getCost());
  //cout << "cost: " << actual.getCost() << endl;
  if ((actual.getCost() < best.getCost())/*&& actual.getCost() > 0*/) {
    best = actual;
    //best = InterRouteLocalSearch(best, vrp, actual.getCost());
    return LocalSearch(best,vrp,method);
  }
  return best;
}

/**
 * @brief Local search intra route with just one switch by insertion
 * 
 * @param old 
 * @param vrp 
 * @param Cost 
 * @return Solution 
 */
Solution Gvns::IntraRouteLocalSearch(Solution old, Vrp vrp, double Cost) {
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
    Vehicles[SwapRoute].setRoute(rt);
    //std::cout << "best local cost: " << Cost << endl;
    //double temp1 = Vehicles[0].getRouteCost(vrp.getDistances());
    //double temp2 = Vehicles[1].getRouteCost(vrp.getDistances());
    //cout << "Coste total: " << temp1 + temp2;
    Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
    //cout << "New disposition: ";
    //sol.checkCost(vrp.getDistances());
    //sol.PrintNodes();
    return sol;
  }
  Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
  return sol;
}

/**
 * @brief Local search intra route with just one switch by swap
 * 
 * @param old 
 * @param vrp 
 * @param Cost 
 * @return Solution 
 */
Solution Gvns::IntraRouteLocalSearchSwap(Solution old, Vrp vrp, double Cost) {
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
      for (int j = i + 1; j < rt.size()-1; j++) {//Not possible to move after last Depot!
        if ( ( j != i ) && (j > 0)) { // Not a move that cHanges solution cost

          double MinusCost1 = CostMatrix[rt[i-1].getId()][rt[i].getId()];
          double MinusCost2 =  CostMatrix[rt[i].getId()][rt[i+1].getId()];
          double MinusCost3 =  CostMatrix[rt[j].getId()][rt[j+1].getId()];
          double MinusCost4 = CostMatrix[rt[j-1].getId()][rt[j].getId()];

          double AddedCost1 = CostMatrix[rt[i-1].getId()][rt[j].getId()];
          double AddedCost2 = CostMatrix[rt[j].getId()][rt[i+1].getId()];
          double AddedCost3 = CostMatrix[rt[j-1].getId()][rt[i].getId()];
          double AddedCost4 = CostMatrix[rt[i].getId()][rt[j+1].getId()];
          if ((j == i+1)) {
            AddedCost2 = CostMatrix[rt[j].getId()][rt[i].getId()];
            MinusCost4 = 0;
          } 
        
          NeigthboorCost = AddedCost1 + AddedCost2 + AddedCost3 + AddedCost4
                  - MinusCost1 - MinusCost2 - MinusCost3 - MinusCost4;
          //cout << "ne cost: " << NeigthboorCost << endl;
          if (NeigthboorCost < BestNCost) {
            //cout << "Swap: " << rt[i].getId() << " with: " << rt[j].getId() << endl;
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
      Vehicles[SwapRoute].setRoute(rt);
      Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
      //sol.checkCost(vrp.getDistances());
      //sol.PrintNodes();
      return sol;
    }
  Solution sol(Vehicles, 0, vrp.getNumberOfCustomers(),Cost,2);
  return sol;
}

/**
 * @brief Local search inter route with just one switch by insertion
 * 
 * @param old 
 * @param vrp 
 * @param Cost 
 * @return Solution 
 */
Solution Gvns::InterRouteLocalSearch(Solution old, Vrp vrp, double Cost) {
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

/**
 * @brief Local search inter route with just one switch by swap
 * 
 * @param old 
 * @param vrp 
 * @param Cost 
 * @return Solution 
 */
Solution Gvns::InterRouteLocalSearchSwap(Solution old, Vrp vrp, double Cost) {
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
        for (VehIndexTo = VehIndexFrom + 1; VehIndexTo < Vehicles.size(); VehIndexTo++) {
          RouteTo =  Vehicles[VehIndexTo].getRoute();
          int RouteTolength = RouteTo.size();
          for (int j = 1; (j < RouteTolength - 1); j++) {//Not possible to move after last Depot!
            if (( (VehIndexFrom == VehIndexTo) && ((j == i) || (j == i - 1)) ) == false)  // Not a move that Changes solution cost
            {
              double MinusCost1 = CostMatrix[RouteFrom[i - 1].getId()][RouteFrom[i].getId()];
              double MinusCost2 = CostMatrix[RouteFrom[i].getId()][RouteFrom[i + 1].getId()];
              double MinusCost3 = CostMatrix[RouteTo[j].getId()][RouteTo[j + 1].getId()];
              double MinusCost4 = CostMatrix[RouteTo[j - 1].getId()][RouteTo[j].getId()];

              double AddedCost1 = CostMatrix[RouteFrom[i - 1].getId()][RouteFrom[j].getId()];
              double AddedCost2 = CostMatrix[RouteTo[j].getId()][RouteFrom[i + 1].getId()];
              double AddedCost3 = CostMatrix[RouteTo[j-1].getId()][RouteFrom[i].getId()];
              double AddedCost4 = CostMatrix[RouteFrom[i].getId()][RouteTo[j+1].getId()];
              if ((j == i+1)) {
                AddedCost2 = CostMatrix[RouteTo[j].getId()][RouteFrom[i].getId()];
                MinusCost4 = 0;
              } 
              NeigthboorCost = AddedCost1 + AddedCost2 + AddedCost3 + AddedCost4
                      - MinusCost1 - MinusCost2 - MinusCost3 - MinusCost4;

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