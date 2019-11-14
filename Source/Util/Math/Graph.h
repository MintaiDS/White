#pragma once

#include <vector>
#include <map>
#include <string>

namespace White {
namespace Util {
namespace Math {

class Edge {
public:
  Edge() {}
  Edge(int idx, int length, int from, int to) : idx(idx), length(length)
  {
    v[0] = from;
    v[1] = to;
  }

  int GetFrom() { return v[0]; }
  int GetTo() { return v[1]; }
  int GetLength() { return length; }

private:
  int idx;
  int length;
  int v[2];
};

class Vertex {

public:
  Vertex() {}
  Vertex(int idx, int post_idx = -1) : idx(idx), post_idx(post_idx) {}

  int GetIdx() { return idx; }

  size_t GetId() { return id; }
  void SetId(size_t id) { this->id = id; }

  void AppendEdge(Edge* e) { edges.push_back(e); }

  std::vector<Edge*>& GetEdgeList() { return edges; }

private:
  size_t id;
  int idx;
  int post_idx;
  std::vector<Edge*> edges;
};

class Graph {
public:
  Graph() {}
  Graph(int idx, std::string name) : idx(idx), name(name) {}
  
  void AppendEdge(Edge* e);
  void AppendVertex(Vertex* v);

  size_t GetVCnt() const;
  Vertex* GetVByIdx(int idx);
  Vertex* GetVById(size_t id);

private:
  int idx;
  std::string name;
  std::vector<Edge*> edges;
  std::vector<Vertex*> vertices;
  std::map<int, size_t> v_ids;
};

Graph* ParseGraphFromJSON(std::string filename);

}
}
}

