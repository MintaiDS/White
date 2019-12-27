#pragma once

#include "Graph.h"
#include "Connection.h"

#include <set>
#include <string>

namespace White {
  namespace Util {
    namespace Math {

      class Overseer {
      public:
        Overseer();
        void Init(std::string playerName);
        void Turn();
        void CheckStatus();
        void AssignTasks();
        void MakeMoves();

        std::shared_ptr<Graph> GetGraph() { return graph; }
      private:
        int food_income;
        std::shared_ptr<Graph> graph;
        City* my_city;
        std::vector<Train*> trains;
        std::set<std::pair<double, Market*>> markets;
        Connection& conn;
        std::string player_idx;

        void GetMyTrains();
        void SortMarkets();
        std::string GetPlayerIdxFromJson(char* data);
        void FindMyCity();
      };
    }
  }
}