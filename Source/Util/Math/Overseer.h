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
        void TryUpgrade();
        void AssignTasks();
        void MakeMoves();

        std::shared_ptr<Graph> GetGraph() { return graph; }
      private:
        int food_income;
        std::shared_ptr<Graph> graph;
        City* my_city;
        std::vector<Train*> trains;
        Connection& conn;
        std::string player_idx;
        int armor_buffer = 10;

        void GetMyTrains();
        std::pair<Train::Task::TaskType, int> ChooseTask(Train* t, int point_id);
        std::string GetPlayerIdxFromJson(char* data);
        void FindMyCity();
        double getFoodPercentage() { return (double)(my_city->GetCurProduct()) / my_city->GetMaxProduct(); }
      };
    }
  }
}