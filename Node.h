#ifndef NODE
#define NODE

class Node {

private:
  int NodeId;
  int demand_;
  bool IsRouted;
  bool isDepot;
public:
  Node(int id, int demand) {NodeId = id;demand_ = demand;isDepot = false; IsRouted = false;}
  ~Node(){}
  int getId() {return NodeId;}
  int getDemand() {return demand_;}
  bool getRouted() {return IsRouted;}
  bool getDepot() {return isDepot;}
  int getId() const{return NodeId;}
  int getDemand() const {return demand_;}
  bool getRouted() const {return IsRouted;}
  bool getDepot() const {return isDepot;}
  void setId(int id) {NodeId = id;}
  void setDemand(int demand) {demand_ = demand;}
  void setRouted(bool routed) {IsRouted = routed;}
  void setDepot(bool depo) {isDepot = depo;}
};

#endif