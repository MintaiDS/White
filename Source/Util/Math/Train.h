#pragma once
#include <string>
#include <vector>

#include "Edge.h"
#include "Post.h"


namespace White {
  namespace Util {
    namespace Math {

      class Train {
      public:
        static const int level_cost[2];
        Train() {}
        Train(int idx, int line_idx, int position, std::string& player_idx) : idx(idx), line_idx(line_idx), position(position), player_idx(player_idx) {}

        enum Goods {
          NONE = 0,
          FOOD = 2,
          ARMOR = 3
        };

        class Task {
        public:
          enum TaskType {
            NO_TASK = 0,
            GATHER_FOOD = 1,
            GATHER_ARMOR = 2,
            KAMIKAZE = 3,
            DEFENDER = 4,
            COME_HOME = 5
          };

          void SetTask(TaskType t, std::vector<std::pair<Edge*, bool>>& path, Post* p, int start_idx);
          std::pair<int, int> ContinueMovement(Edge* edge, int position);

          std::pair<Edge*, bool> GetPathPart(int i) { return path[i]; }

          TaskType GetTask() { return task; }
          bool TaskEnded(int line_idx, int position);
          void DropTask();
          bool PathIdxValid(Train* t);
          //for evasive movement
          void ChangeCurrentPath(std::pair<Edge*, bool> new_path);
          int GetPathIdx() { return path_idx; }
          int GetPathLen() { return path.size(); }
          void SetPathIdx(int val) { path_idx = val; }
          void DestroyStartIdx() { start_idx = -1; }
          bool IsFirst(int position);
          Post* GetDestination() { return destination; }
        private:
          int path_idx;
          TaskType task = NO_TASK;
          Post* destination;
          int start_idx;
          std::vector<std::pair<Edge*, bool>> path;
          bool go_back = false;
        };

        Task task;
        Train* block = NULL;
        int GetIdx() { return idx; }
        int GetPosition() { return position; }
        int GetLineIdx() { return line_idx; }
        int GetPrevPosition() { return prev_position; }
        int GetPrevLineIdx() { return prev_line_idx; }
        int GetGoods() { return goods; }
        int GetGoodsCap() { return goodsCapacity; }
        Goods GetGoodsType() { return goodsType; }
        std::string& GetPlayerIdx() { return player_idx; }
        Task::TaskType GetTask() { return task.GetTask(); }
        int GetLevel() { return level; }
        int GetDirection() { return direction; }
        bool IsMoved() { return moved; }
        std::pair<int, int> GetMove() { return move; }
        int GetDecCooldown();
        int GetCooldown() { return cooldown; }
        bool IsMessageNeeded() { return need_message; }
        double GetFoodIncome() { return food_income; }
        
        void SetIdx(int idx) { this->idx = idx; }
        void SetPosition(int position) { this->position = position; }
        void SetLineIdx(int line_idx) { this->line_idx = line_idx; }
        void SetPrevPosition(int position) { prev_position = position; }
        void SetPrevLineIdx(int line_idx) { prev_line_idx = line_idx; }
        void SetPlayerIdx(std::string& player_idx) { this->player_idx = player_idx; }
        void SetGoods(int val) { goods = val; }
        void IncGoods(int val) { goods += val; }
        void SetGoodsType(Goods type) { goodsType = type; }
        void SetGoodsCap(int val) { goodsCapacity = val; }
        void SetLevel(int val) { level = val; }
        void SetMoved(bool val) { moved = val; }
        void SetDirection(int val) { direction = val; }
        void SetMove(std::pair<int, int> val) { move = val; }
        void SetNeedMessage(bool val) { need_message = val; }
        void SetFoodIncome(double val) { food_income = val; }
        
        void ResetCooldown();

        bool IncStopCount();
        void ResetStopCount() { stop_count = 0; }

      private:
        int idx;
        int line_idx;
        int prev_line_idx = 0;
        int position;
        int prev_position = 0;
        std::string player_idx;
        Goods goodsType;
        int goodsCapacity;
        int goods;
        int level;
        int direction;
        bool moved;
        bool need_message;
        std::pair<int, int> move = { 1, 1 };
        int cooldown = 0;
        double food_income = 0.;
        int stop_count = 0;

      };
    }
  }
}
