#include "json.hpp"
#include "Graph.h"
#include "Algo.h"
#include <fstream>
#include "Logger.h"
#include <cassert>

namespace White {
namespace Util {
namespace Math {

using json = nlohmann::json;

void Vertex::SetSize(int x, int y)
{
  coord.x = x;
  coord.y = y;
}

void Graph::AppendEdge(Edge* e)
{
  Vertex* v = GetVByIdx(e->GetFrom());
  v->AppendEdge(e);
  v = GetVByIdx(e->GetTo());
  v->AppendEdge(e);
  edges[e->GetIdx()] = e;
}

void Graph::AppendVertex(Vertex* v)
{
  v->SetId(vertices.size());
  v_ids[v->GetIdx()] = v->GetId();
  vertices.push_back(v);
}

void Graph::AppendTrain(Train * t)
{
  trains[t->GetIdx()] = t;
}

void Graph::AppendPost(Post * p)
{
  int type = p->GetPostType();
  switch (type)
  {
  case PostType::CITY:
    cities[p->GetIdx()] = (City*)p;
    break;
  case PostType::MARKET:
    markets[p->GetIdx()] = (Market*)p;
    break;
  case PostType::STORAGE:
    storages[p->GetIdx()] = (Storage*)p;
    break;
  }
  Vertex* v = GetVByIdx(p->GetPointIdx());
  v->SetPost(p);
}

Vertex* Graph::GetVByIdx(int idx)
{
  size_t id = GetIdByIdx(idx);
  return id != -1 ? vertices[id] : nullptr;
}

size_t Graph::GetIdByIdx(int idx)
{
  std::map<int, size_t>::iterator it;
  if ((it = v_ids.find(idx)) != v_ids.end())
  {
    return it->second;
  }
  return -1;
}

void Graph::SetSize(int x, int y)
{
  map_size.x = x;
  map_size.y = y;
}

void Graph::InitWorldPaths()
{
  size_t sz = vertices.size();
  world_map.resize(sz);
  distance.resize(sz, std::vector<int>(sz, -1));
}

void Graph::FillWorldPaths()
{
  size_t size_v = vertices.size();
  for (int i = 0; i < size_v; ++i)
  {
    world_map[i] = Dijkstra(*this, i, NULL, NULL);
  }
}

void Graph::FillWorldPath(int i)
{
  world_map[i] = Dijkstra(*this, i, NULL, NULL);
}

std::vector<std::pair<Edge*, bool>> Graph::GetPath(int i_idx, int j_idx)
{
  int i_id = GetIdByIdx(i_idx);
  int j_id = GetIdByIdx(j_idx);
  if (world_map[i_id].size() == 0)
    FillWorldPath(i_id);
  std::vector<std::pair<Edge*, bool>> res;
  while (j_id != i_id)
  {
    std::pair<Edge*, bool> edge = world_map[i_id][j_id];
    res.push_back(edge);
    j_idx = vertices[j_id]->GetIdx();
    j_idx = edge.first->GetOtherV(j_idx);
    j_id = GetIdByIdx(j_idx);
  }
  res.shrink_to_fit();
  size_t sz = res.size();
  for (int i = 0; i < sz / 2; ++i)
  {
    std::swap(res[i], res[sz - 1 - i]);
  }
  return res;
}

std::vector<std::pair<Edge*, bool>> Graph::GetPathReverse(int i_idx, int j_idx)
{
  int i_id = GetIdByIdx(i_idx);
  int j_id = GetIdByIdx(j_idx);
  assert(world_map[j_id].size() != 0);
  std::vector<std::pair<Edge*, bool>> res;
  while (i_id != j_id)
  {
    std::pair<Edge*, bool> edge = world_map[j_id][i_id];
    res.push_back(edge);
    i_idx = vertices[i_id]->GetIdx();
    i_idx = edge.first->GetOtherV(i_idx);
    i_id = GetIdByIdx(i_idx);
  }
  res.shrink_to_fit();
  size_t sz = res.size();
  for (int i = 0; i < sz; ++i)
  {
    res[i].second = !res[i].second;
  }
  return res;
}

int Graph::GetPathLen(int i_idx, int j_idx)
{
  int i_id = GetIdByIdx(i_idx);
  int j_id = GetIdByIdx(j_idx);
  if (distance[i_id][j_id] != -1)
    return distance[i_id][j_id];
  if (world_map[i_id].size() == 0)
    FillWorldPath(i_id);
  int res = 0;
  while (j_id != i_id)
  {
    std::pair<Edge*, bool> edge = world_map[i_id][j_id];
    res += edge.first->GetLength();
    j_idx = vertices[j_id]->GetIdx();
    j_idx = edge.first->GetOtherV(j_idx);
    j_id = GetIdByIdx(j_idx);
  }
  distance[i_id][j_id] = distance[j_id][i_id] = res;
  return res;
}

std::shared_ptr<Graph> ParseGraphFromJSONFile(std::string filename)
{
  std::ifstream f_inp;
  f_inp.open(filename, std::ifstream::in);
  std::string result = "", buf;
  while (!f_inp.eof())
  {
    std::getline(f_inp, buf);
    result += buf;
  }
  std::shared_ptr<Graph> g = std::make_shared<Graph>();
  char* data = (char*)result.c_str();
  ParseGraphFromJSON(g, data);
  return g;
}

void ParseGraphFromJSON(std::shared_ptr<Graph> g, char* data)
{
  json json_parsed = json::parse(data);
  int idx = json_parsed.at("idx");
  std::string name = json_parsed.at("name");
  g->SetIdx(idx);
  g->SetName(name);
  json v_arr = json_parsed.at("points");
  size_t v_size = v_arr.size();
  for (size_t i = 0; i < v_size; ++i)
  {
    json v_j = v_arr[i];
    int v_idx = v_j.at("idx");
    int v_post_idx = v_j["post_idx"].is_null() ? -1 : v_j.at("post_idx");
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
}

void ParseCoordFromJSON(std::shared_ptr<Graph> g, char* data)
{
  json json_parsed = json::parse(data);
  json coord_j = json_parsed.at("coordinates");
  int idx = json_parsed.at("idx");
  json size_j = json_parsed.at("size");
  g->SetSize(size_j[0], size_j[1]);
  for (int i = 0; i < coord_j.size(); ++i)
  {
    idx = coord_j[i].at("idx");
    Vertex* v = g->GetVByIdx(idx);
    assert(v != nullptr);
    v->SetSize(coord_j[i].at("x"), coord_j[i].at("y"));
  }
}

void ParseInfrastructureFromJSON(std::shared_ptr<Graph> g, char* data)
{
  Logger& l = Logger::GetInstance();
  json json_parsed = json::parse(data);
  json posts_j = json_parsed.at("posts");
  json trains_j = json_parsed.at("trains");
  for (int i = 0; i < posts_j.size(); ++i)
  {
    json post_j = posts_j[i];
    int type = post_j.at("type");
    int idx = post_j.at("idx");
    int point_idx = post_j.at("point_idx");
    std::string s;
    switch (type)
    {
    case PostType::CITY:
    {
      s = post_j["player_idx"].get<std::string>();
      City* c = new City(idx, point_idx, s);
      c->SetCurProduct(post_j["product"]);
      c->SetMaxProduct(post_j["product_capacity"]);
      c->SetCurArmor(post_j["armor"]);
      c->SetMaxArmor(post_j["armor_capacity"]);
      g->AppendPost(c);
      break;
    }
    case PostType::MARKET:
    {
      Market* m = new Market(idx, point_idx);
      m->SetCurProduct(post_j["product"]);
      m->SetMaxProduct(post_j["product_capacity"]);
      g->AppendPost(m);
      break;
    }
    case PostType::STORAGE:
    {
      Storage* st = new Storage(idx, point_idx);
      st->SetCurArmor(post_j["armor"]);
      st->SetMaxArmor(post_j["armor_capacity"]);
      g->AppendPost(st);
      break;
    }
    }
  }
  for (int i = 0; i < trains_j.size(); ++i)
  {
    json train_j = trains_j[i];
    int idx = train_j.at("idx");
    int line_idx = train_j.at("line_idx");
    int position = train_j.at("position");
    std::string s = train_j.at("player_idx");
    Train* t = new Train(idx, line_idx, position, s);
    int goods = train_j.at("goods");
    int goods_cap = train_j.at("goods_capacity");
    t->SetGoodsCap(goods_cap);
    t->SetGoods(goods);
    t->SetGoodsType(Train::Goods::NONE);
    g->AppendTrain(t);
  }
}

void UpdateInfrastructureFromJSON(std::shared_ptr<Graph> g, char* data)
{
  Logger& l = Logger::GetInstance();
  json json_parsed = json::parse(data);
  json posts_j = json_parsed.at("posts");
  json trains_j = json_parsed.at("trains");
  for (int i = 0; i < posts_j.size(); ++i)
  {
    json post_j = posts_j[i];
    int type = post_j.at("type");
    int idx = post_j.at("idx");
    std::string s;

    switch (type)
    {
    case PostType::CITY:
    {
      City* c = g->GetCityByIdx(idx);
      c->SetCurProduct(post_j["product"]);
      c->SetCurArmor(post_j["armor"]);
      break;
    }
    case PostType::MARKET:
    {
      Market* m = g->GetMarketByIdx(idx);
      m->SetCurProduct(post_j["product"]);
      break;
    }
    case PostType::STORAGE:
    {
      Storage* st = g->GetStorageByIdx(idx);
      st->SetCurArmor(post_j["armor"]);
      break;
    }
    }
  }
  for (int i = 0; i < trains_j.size(); ++i)
  {
    json train_j = trains_j[i];
    int idx = train_j.at("idx");
    int line_idx = train_j.at("line_idx");
    int position = train_j.at("position");
    int goods = train_j.at("goods");
    int goods_cap = train_j.at("goods_capacity");
    json j_type = train_j.at("goods_type");
    std::string type;
    if (j_type.is_null())
      type = "null";
    else
      type = std::to_string(j_type.get<int>());
    //l << type;
    //l << std::string("\n");
    Train::Goods g_type = Train::Goods::NONE;
    if (type == "null")
      g_type = Train::Goods::NONE;
    else if (type == "1")
      g_type = Train::Goods::FOOD;
    else if (type == "2")
      g_type = Train::Goods::ARMOR;
    Train* t = g->GetTrainByIdx(idx);
    t->SetLineIdx(line_idx);
    t->SetPosition(position);
    t->SetGoods(goods);
    t->SetGoodsCap(goods_cap);
    t->SetGoodsType(g_type);
  }
}

}
}
}

