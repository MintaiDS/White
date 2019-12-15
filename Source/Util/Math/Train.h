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
          std::pair<int, int>* ContinueMovement(Edge* edge, int position);

          TaskType GetTask() { return task; }
          bool TaskEnded(int line_idx, int position);
          void DropTask();
          void CheckPathIdx(int line_idx);
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
        int GetIdx() { return idx; }
        int GetPosition() { return position; }
        int GetLineIdx() { return line_idx; }
        int GetGoods() { return goods; }
        int GetGoodsCap() { return goodsCapacity; }
        Goods GetGoodsType() { return goodsType; }
        std::string& GetPlayerIdx() { return player_idx; }
        Task::TaskType GetTask() { return task.GetTask(); }
        int GetLevel() { return level; }

        void SetIdx(int idx) { this->idx = idx; }
        void SetPosition(int position) { this->position = position; }
        void SetLineIdx(int line_idx) { this->line_idx = line_idx; }
        void SetPlayerIdx(std::string& player_idx) { this->player_idx = player_idx; }
        void SetGoods(int val) { goods = val; }
        void IncGoods(int val) { goods += val; }
        void SetGoodsType(Goods type) { goodsType = type; }
        void SetGoodsCap(int val) { goodsCapacity = val; }
        void SetLevel(int val) { level = val; }

      private:
        int idx;
        int line_idx;
        int position;
        std::string player_idx;
        Goods goodsType;
        int goodsCapacity;
        int goods;
        int level;
      };
    }
  }
}
