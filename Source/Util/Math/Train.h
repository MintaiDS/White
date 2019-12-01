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
        Train() {}
        Train(int idx, int line_idx, int position, std::string& player_idx) : idx(idx), line_idx(line_idx), position(position), player_idx(player_idx) {}

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

          void SetTask(TaskType t, std::vector<std::pair<Edge*, bool>>& path, Post* p);
          std::pair<int, int>* ContinueMovement(int line_idx, int position);

          TaskType GetTask() { return task; }
          bool TaskEnded(int line_idx, int position);
          void DropTask() { task = NO_TASK; }
          void CheckPathIdx(int line_idx);
        private:
          int path_idx;
          TaskType task = NO_TASK;
          Post* destination;
          std::vector<std::pair<Edge*, bool>> path;
          bool go_back = false;
        };

        Task task;
        int GetIdx() { return idx; }
        int GetPosition() { return position; }
        int GetLineIdx() { return line_idx; }
        std::string& GetPlayerIdx() { return player_idx; }

        void SetIdx(int idx) { this->idx = idx; }
        void SetPosition(int position) { this->position = position; }
        void SetLineIdx(int line_idx) { this->line_idx = line_idx; }
        void SetPlayerIdx(std::string& player_idx) { this->player_idx = player_idx; }
        Task::TaskType GetTask() { return task.GetTask(); }

      private:
        int idx;
        int line_idx;
        int position;
        std::string player_idx;
      };
    }
  }
}
