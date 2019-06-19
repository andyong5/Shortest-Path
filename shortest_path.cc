#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <float.h>
#include <stack>
#include "indexMin.h"
class Edge {
 public:
  Edge(int src, int des, double weight) : src(src), des(des), weight(weight) {};
  int getSrc() const {
    return src;
  }
  int getDes() const {
    return des;
  }
  double getWeight() const {
    return weight;
  }
 private:
  int src;
  int des;
  double weight;
};
class shortestPath {
 private:

};
class Vertex {
 public:
  Vertex(int vertex) : vertex(vertex) {
  }
  std::vector<Edge *> edges;
  int vertex;
};
class Graph {
 public:
  Graph(const char *graphFile, int src, int dst, int &numVert);
  std::vector<Vertex *> &getVec() { return vecGrph; }
  std::vector<Vertex *> vecGrph;
};
Graph::Graph(const char *graphFile, int src, int dst, int &numVert) {
  int vert, dest;
  double weight;
  std::ifstream file;
  file.open(graphFile);

  if (!file.good()) {
    std::cerr << "Error: cannot open file wrong_file" << graphFile << std::endl;
    exit(1);
  }

  if(!(file>>numVert)) {
    std::cerr << "Error: invalid graph size" << std::endl;
    exit(1);
  }
  else if (src >= numVert) {
    std::cerr << "Error: invalid source vertex number " << src << std::endl;
    exit(1);
  }
  if (dst >= numVert) {
    std::cerr << "Error: invalid dest vertex number " << dst << std::endl;
    exit(1);
  }
  std::vector<Edge *> vec;
  this->vecGrph.resize(numVert);
  for (int i = 0; i < numVert; i++) {
    this->vecGrph[i] = new Vertex(i);
  }
  bool check=true;
  while (file >> vert >> dest >> weight) {
    check=false;
    if (weight < 0) {
      std::cerr <<"Invalid weight " << weight << std::endl;
      exit(1);
    }
    if (vert > numVert) {
      std::cerr << "Invalid source vertex number " << vert << std::endl;
      exit(1);
    }
    if (dest > numVert) {
      std::cerr << "Invalid dest vertex number " << dest << std::endl;
      exit(1);
    }
    this->vecGrph[vert]->edges.push_back(new Edge(vert, dest, weight));
  }
  if(check)
  {
    std::cout<<src<<" to "<<dst<<": no path"<<std::endl;
    exit(0);
  }
}
bool checkInt(std::string response) {
  std::istringstream s(response);
  double check;
  if (s >> check)
    return true;
  return false;
}
void findShortestPath(Graph &g, int src, int dst) {
  IndexMinPQ<double> Q(g.vecGrph.size());
  std::vector<double> dist;
  std::vector<double> prev;
  double u, alt;

  for (int i = 0; i < g.vecGrph.size(); i++) {
    dist.push_back(DBL_MAX);
    prev.push_back(-1);
  }

  int v = src;
  dist[src] = 0;
  Q.Push(dist[src], src);

  while (!Q.isEmpty()) {
    u = Q.Top();
    Q.Pop();

    if (u == dst)
      break;
    int placeHolder = v;
    for (int c = 0; c < g.vecGrph[placeHolder]->edges.size(); c++) {
      alt = dist[u] + g.vecGrph[placeHolder]->edges[c]->getWeight();
      v = g.vecGrph[placeHolder]->edges[c]->getDes();
      if (alt < dist[v]) {
        dist[v] = alt;
        prev[v] = u;
        if (Q.Contains(v))
          Q.ChangeKey(dist[v], v);
        else {
          Q.Push(dist[v], v);
        }

      }
    }
  }
  double total = 0;
  std::stack<double> S;
  u = dst;
  if (dist[u] != DBL_MAX) {
    while (u != -1) {
      S.push(u);
      u = prev[u];
    }
  }
  std::cout << src << " to " << dst << ": ";
  while (!S.empty()) {
    if (S.size() > 1) {
      std::cout << S.top() << " => ";
      S.pop();
    } else {
      std::cout << S.top();
      total += dist[S.top()];
      S.pop();
    }
  }
  std::cout << " (" << total << ")" << std::endl;
}
int main(int argc, char *argv[]) {
  int numVert = 0;
  int src = 0;
  int dest = 0;

  if (argc < 4) {
    std::cerr << "Usage: ./shortest_path "
                 "<graph.dat> src dst" << std::endl;
    exit(1);
  }

  if (checkInt(argv[2])) {
    src = std::stoi(argv[2]);
    if (src < 0) {
      std::cerr << "Error: invalid source vertex number " << src << std::endl;;
      exit(1);
    }
  } else {
    std::cerr << "Error: invalid source vertex number " << argv[2] << std::endl;;
    exit(1);
  }
  if (checkInt(argv[3])) {
    dest = std::stoi(argv[3]);
    if (dest < 0) {
      std::cerr << "Error: invalid dest vertex number " << dest << std::endl;;
      exit(1);
    }
  } else {
    std::cerr << "Error: invalid dest vertex number "
              << argv[3] << std::endl;
    exit(1);
  }
  Graph g(argv[1], src, dest, numVert);
  findShortestPath(g, src, dest);
  return 0;
}
