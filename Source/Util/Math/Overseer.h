#pragma once

#include "Graph.h"
#include "Connection.h"

#include <set>
#include <map>
#include <string>
#include <time.h>

namespace White {
  namespace Util {
    namespace Math {

      class Overseer {
      public:
        Overseer();
        void Init(std::string playerName, std::string game, std::string num_players, std::string num_turns);
        bool Turn();
        void CheckStatus();
        void TryUpgrade();
        void AssignTasks();
        void MakeMoves();
        void TryMakeMove(Train* t);
        bool MakeEvasionMove(Train* t, int point_idx, int locked_edge_from, int locked_edge_to);
        Train* CheckForCollision(Train* t, int edge_idx, int position);
        void Logout();

        int GetRating() { return rating; }
        void ParseGraphFromJSON(char* data);
        void ParseCoordFromJSON(char* data);
        void ParseInfrastructureFromJSON(char* data);
        void UpdateInfrastructureFromJSON(char* data);


        std::shared_ptr<Graph> GetGraph() { return graph; }
      private:

        enum GameState
        {
          INIT = 1,
          RUN = 2,
          FINISHED = 3
        };

        const int sleep_time = 100;

        std::string game_name;
        //double food_income = 0.;
        std::shared_ptr<Graph> graph;
        City* my_city;
        std::vector<Train*> trains;
        Connection& conn;
        std::string player_idx;
        int armor_buffer = 19;
        int rating = 0;
        clock_t prev_clock = 0;


        void GetMyTrains();
        bool IsCityCollision(Train* t, Train* tr, std::pair<int, int> step);
        std::pair<Train::Task::TaskType, int> ChooseTask(Train* t, int point_id);
        void ParseLogin(char* data);
        void FindMyCity();
        double GetFoodPercentage() { return (double)(my_city->GetCurProduct()) / my_city->GetMaxProduct(); }
        void CalcFoodIncome();
        void CollisionCleanup(Train * t, bool need_reset);
        double CountFoodIncome();
        int CheckSurroundings(int point_idx, Train* tr);
        bool CheckCycle(Train* cur, Train* start);

        void SetRating(int val) { rating = val; }

        GameState ParseGameState(char* data);
        std::shared_ptr<Graph> ParseGraphFromJSONFile(std::string filename);
      };
    }
  }
}