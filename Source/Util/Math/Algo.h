#pragma once

#include "Graph.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include "Logger.h"

namespace White {
  namespace Util {
    namespace Math {

      std::vector<std::pair<Edge*, bool>> Dijkstra(Graph& g, int id, const std::map<int, int>* forbidden_e, std::map<int, int>* forbidden_v);
      void PlaceVertices(std::shared_ptr<Graph> g);
    }
  }
}