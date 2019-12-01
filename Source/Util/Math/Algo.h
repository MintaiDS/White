#pragma once

#include "Graph.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <map>

namespace White {
  namespace Util {
    namespace Math {

      std::vector<std::pair<Edge*, bool>> Dijkstra(Graph& g, int id, const std::map<int, int>* forbidden_e, std::map<int, int>* forbidden_v)
      {
        std::vector<Vertex*> vrtx = g.GetVertices();
        size_t num_v = vrtx.size();
        std::vector<int> dist(num_v, INT_MAX);
        std::vector<std::vector<int>> edg(num_v);
        std::vector<std::pair<Edge*, bool>> p;

        std::priority_queue<std::pair<int, int>> q;
        int idx = vrtx[id]->GetIdx();
        dist[id] = 0;
        q.push(std::make_pair(0, idx));
        while (!q.empty()) {
          int v = q.top().second;
          int cur_d = -q.top().first;
          q.pop();
          size_t v_id = g.GetIdByIdx(v);
          if (cur_d > dist[v_id])
            continue;
          std::vector<Edge*> edges = vrtx[v_id]->GetEdgeList();
          for (size_t j = 0; j < edges.size(); ++j) {
            Edge* e = edges[j];
            if (forbidden_e == NULL || forbidden_e->find(e->GetIdx()) == forbidden_e->end())
            {
              int to = e->GetOtherV(v);
              if (forbidden_v == NULL || forbidden_v->find(to) == forbidden_v->end())
              {
                int len = e->GetLength();
                size_t to_id = g.GetIdByIdx(to);
                if (dist[v_id] + len < dist[to_id]) {
                  dist[to_id] = dist[v_id] + len;
                  p[to_id] = { e, to == e->GetTo() ? true : false };
                  q.push(std::make_pair(-dist[to_id], to));
                }
              }
            }
          }
        }
        return p;
      }
    }
  }
}