#include "json.hpp"
#include "Graph.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

void Graph::AppendEdge(Edge* e) 
{
  Vertex* v = GetVByIdx(e->GetFrom());
  v->AppendEdge(e);
  v = GetVByIdx(e->GetTo());
  v->AppendEdge(e);
  edges.push_back(e); 
}

void Graph::AppendVertex(Vertex* v)
{
  v->SetId(vertices.size());
  v_ids[v->GetIdx()] = v->GetId();
  vertices.push_back(v); 
}

Vertex* Graph::GetVByIdx(int idx)
{
  std::map<int, size_t>::iterator it;
  if ((it = v_ids.find(idx)) != v_ids.end())
  {
    return vertices[it->second];
  }
  return nullptr;
}

Graph* ParseGraphFromJSON(std::string filename)
{
  std::ifstream f_inp;
  f_inp.open(filename, std::ifstream::in);
  json json_parsed = json::parse(f_inp);
  f_inp.close();

  int idx = json_parsed.at("idx");
  std::string name = json_parsed.at("name");
  Graph* g = new Graph(idx, name);

  json v_arr = json_parsed.at("points");
  size_t v_size = v_arr.size();
  for (size_t i = 0; i < v_size; ++i)
  {
    json v_j = v_arr[i];
    int v_idx = v_j.at("idx");
    int v_post_idx = v_j["post_idx"].is_null() ? -1 :v_j.at("post_idx");
    Vertex* v = new Vertex(v_idx, v_post_idx);
    g->AppendVertex(v);
  }

  json e_arr = json_parsed.at("lines");
  size_t e_size = e_arr.size();
  for (size_t i = 0; i < e_size; ++i)
  {
    json e_j = e_arr[i];
    int e_idx = e_j.at("idx");
    int e_len = e_j.at("length");
    json vertices = e_j.at("points");
    Edge* e = new Edge(e_idx, e_len, vertices[0], vertices[1]);
    g->AppendEdge(e);
  }
  return g;
}


//int main()
//{
//  Graph* g = ParseGraphFromJSON("big_graph.json");
//  return 0;
//}
