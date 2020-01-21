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
        coord[0] = (float)x;
        coord[1] = (float)y;
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

      void Graph::InitWorldPaths(int my_city_idx)
      {
        Logger& l = Logger::GetInstance();
        int my_city_id = GetIdByIdx(my_city_idx);
        size_t sz = vertices.size();
        world_map.resize(sz, std::vector<Path*>(sz, nullptr));
        std::map<int, int> storages;
        for (auto& s : GetStorages())
          storages[s.second->GetPointIdx()] = 1;
        auto dijkstra_m = Dijkstra(*this, my_city_idx, NULL, &storages);
        for (auto& m : GetMarkets())
        {
          Path* path = new Path();
          int market_idx = m.second->GetPointIdx();
          *path = Path::FormPath(*this, my_city_idx, market_idx, dijkstra_m);
          world_map[my_city_id][GetIdByIdx(market_idx)] = path;
        }

        std::map<int, int> markets;
        for (auto& m : GetMarkets())
          markets[m.second->GetPointIdx()] = 1;
        auto dijkstra_s = Dijkstra(*this, my_city_idx, NULL, &markets);
        for (auto& s : GetStorages())
        {
          Path* path = new Path();
          int storage_idx = s.second->GetPointIdx();
          *path = Path::FormPath(*this, my_city_idx, storage_idx, dijkstra_s);
          world_map[my_city_id][GetIdByIdx(storage_idx)] = path;
        }
        auto dijkstra = Dijkstra(*this, my_city_idx, NULL, NULL);
        for (size_t i = 0; i < sz; ++i)
        {
          if (world_map[my_city_id][i] == nullptr)
          {
            Path* path = new Path();
            int idx = vertices[i]->GetIdx();
            *path = Path::FormPath(*this, my_city_idx, idx, dijkstra);
            world_map[my_city_id][i] = path;
          }
        }
        //distance.resize(sz, std::vector<int>(sz, -1));
      }

      //void Graph::FillWorldPaths()
      //{
      //  size_t size_v = vertices.size();
      //  for (int i = 0; i < size_v; ++i)
      //  {
      //    world_map[i] = Dijkstra(*this, i, NULL, NULL);
      //  }
      //}

      void Graph::FillWorldPath(int idx)
      {
        Logger& l = Logger::GetInstance();
        auto dijkstra = Dijkstra(*this, idx, NULL, NULL);
        int sz = GetVerticesCnt();
        for (int i = 0; i < sz; ++i)
        {
          Path* path = new Path();
          int idx_v = vertices[i]->GetIdx();
          *path = Path::FormPath(*this, idx, idx_v, dijkstra);
          world_map[GetIdByIdx(idx)][i] = path;
        }
        //world_map[i] = Dijkstra(*this, i, NULL, NULL);
      }

      std::vector<std::pair<Edge*, bool>> Graph::GetPath(int i_idx, int j_idx)
      {
        int i_id = GetIdByIdx(i_idx);
        int j_id = GetIdByIdx(j_idx);
        if (world_map[i_id][j_id] == nullptr)
          FillWorldPath(i_idx);
        Path* res = world_map[i_id][j_id];
        return res->GetPath();
      }

      //std::vector<std::pair<Edge*, bool>> Graph::GetPathReverse(int i_idx, int j_idx)
      //{
      //  int i_id = GetIdByIdx(i_idx);
      //  int j_id = GetIdByIdx(j_idx);
      //  assert(world_map[j_id].size() != 0);
      //  std::vector<std::pair<Edge*, bool>> res;
      //  while (i_id != j_id)
      //  {
      //    std::pair<Edge*, bool> edge = world_map[j_id][i_id];
      //    res.push_back(edge);
      //    i_idx = vertices[i_id]->GetIdx();
      //    i_idx = edge.first->GetOtherV(i_idx);
      //    i_id = GetIdByIdx(i_idx);
      //  }
      //  res.shrink_to_fit();
      //  size_t sz = res.size();
      //  for (int i = 0; i < sz; ++i)
      //  {
      //    res[i].second = !res[i].second;
      //  }
      //  return res;
      //}

      int Graph::GetPathLen(int i_idx, int j_idx)
      {
        Logger& l = Logger::GetInstance();
        int i_id = GetIdByIdx(i_idx);
        int j_id = GetIdByIdx(j_idx);
        if (world_map[i_id][j_id] == nullptr)
          FillWorldPath(i_idx);
        Path* res = world_map[i_id][j_id];
        return res->GetDist();
      }

      int Graph::GetCommonPointIdx(int edge_idx1, int edge_idx2)
      {
        Logger& l = Logger::GetInstance();
        Edge* e1 = GetEdgeByIdx(edge_idx1);
        Edge* e2 = GetEdgeByIdx(edge_idx2);
        int from1 = e1->GetFrom();
        int to1 = e1->GetTo();
        int from2 = e2->GetFrom();
        int to2 = e2->GetTo();
        l << edge_idx1 << from1 << to1;
        l << edge_idx2 << from2 << to2;
        if (from1 == from2 || from1 == to2)
          return from1;
        if (to1 == from2 || to1 == to2)
          return to1;
        return -1;
      }

      int Graph::GetPointIdxByPosition(int edge_idx, int position)
      {
        return GetEdgeByIdx(edge_idx)->GetPointIdxFromPosition(position);
      }

      int Graph::GetCloserPoint(int edge_idx, int position)
      {
        Edge* e = GetEdgeByIdx(edge_idx);
        int len = e->GetLength();
        if (position < len - position)
          return e->GetFrom();
        else
          return e->GetTo();
      }

      Path Path::FormPath(Graph& g, int i_idx, int j_idx, std::vector<std::pair<Edge*, bool>>& dijkstra)
      {
        int i_id = g.GetIdByIdx(i_idx);
        int j_id = g.GetIdByIdx(j_idx);
        std::vector<std::pair<Edge*, bool>> res;
        int dist = 0;
        while (j_id != i_id)
        {
          std::pair<Edge*, bool> edge = dijkstra[j_id];
          res.push_back(edge);
          dist += edge.first->GetLength();
          j_idx = g.GetVById(j_id)->GetIdx();
          j_idx = edge.first->GetOtherV(j_idx);
          j_id = g.GetIdByIdx(j_idx);
        }
        res.push_back({ NULL, false });
        res.shrink_to_fit();
        size_t sz = res.size();
        for (size_t i = 0; i < sz / 2; ++i)
        {
          std::swap(res[i], res[sz - 1 - i]);
        }
        return Path(res, dist);
      }

      Train * Graph::CheckLine(int edge_idx)
      {
        std::map<int, Train*>::iterator it;
        if ((it = blocked_lines.find(edge_idx)) != blocked_lines.end())
          return it->second;
        else
          return NULL;
      }

      void Graph::BlockLine(int edge_idx, Train * t)
      {
        blocked_lines[edge_idx] = t;
      }

      void Graph::UnblockLine(int edge_idx, Train* t)
      {
        std::map<int, Train*>::iterator it;
        if ((it = blocked_lines.find(edge_idx)) != blocked_lines.end() && it->second == t)
          blocked_lines.erase(edge_idx);
      }
    }
  }
}

