#include "Train.h"
#include "Logger.h"
#include "Graph.h"

#include <cassert>
namespace White {
  namespace Util {
    namespace Math {

      const int Train::level_cost[2] = { 30, 60 };

      void Train::Task::SetTask(TaskType t, std::vector<std::pair<Edge*, bool>>& path, Post* p, int start_idx)
      {
        Logger& l = Logger::GetInstance();
        if (t == GATHER_FOOD)
          l << std::string("Gather food\n");
        if (t == GATHER_ARMOR)
          l << std::string("Gather armor\n");
        if (t == COME_HOME)
          l << std::string("Come home\n");
        task = t;
        this->path = path;
        for (size_t i = 0; i < path.size(); ++i)
        {
          if (path[i].first != NULL)
            l << std::to_string(path[i].first->GetIdx()) << std::string(" ") << std::to_string(path[i].second) << std::string(", ");
          else
            l << std::string("Empty, ");
          l << std::string("\n");
        }
        this->start_idx = start_idx;
        //go_back = 0;
        path_idx = 1;
        destination = p;
        p->SetVacant(false);
      }

      std::pair<int, int> Train::Task::ContinueMovement(Edge* edge, int position)
      {
        Logger& l = Logger::GetInstance();
        //l << std::string("path_size: ") + std::to_string(path.size()) + std::string("\n");
        //l << std::string("path_idx: ") + std::to_string(path_idx) + "\n";
        int line_idx = edge->GetIdx();
        if (path.size() == 1)
          return { line_idx, 0 };
        //assert(task != NO_TASK);
        Edge* e = path[path_idx].first;
        bool is_back = path[path_idx].second;
        if (position == edge->GetLength() || position == 0)
        {
          int idx;
          if (position == 0)
          {
            idx = edge->GetFrom();
          }
          else
            idx = edge->GetTo();
          if (idx == start_idx)
          {
            std::pair<int, int> step = { e->GetIdx(), path[path_idx].second ? -1 : 1 };
            return step;
          }
        }
        if ((is_back && position == 0) || (!is_back && position == e->GetLength()))
        {
          if (path_idx == path.size() - 1)
            return { e->GetIdx(), is_back ? -1 : 1 };
          //assert(path_idx != path.size());
          Edge* e_next = path[path_idx + 1].first;
          bool is_next_back = path[path_idx + 1].second;
          if (is_next_back)
          {
            std::pair<int, int>step = { e_next->GetIdx(), -1 };
            return step;
          }
          else
          {
            std::pair<int, int> step = { e_next->GetIdx(), 1 };
            return step;
          }
        }
        std::pair<int, int> step = {e->GetIdx(), is_back ? -1 : 1};
        return step;
      }
      bool Train::Task::TaskEnded(int line_idx, int position)
      {
        Logger& l = Logger::GetInstance();
        if (task == NO_TASK)
          return false;
        if (path.size() == 1)
          return true;
        if (path_idx == path.size() - 1)
        {
          if (path[path_idx].second)
            return position == 0;
          else
            return position == path[path_idx].first->GetLength();
        }
        return false;
      }

      void Train::Task::DropTask() 
      { 
        Logger& l = Logger::GetInstance();
        l << std::string("task dropped\n");
        task = NO_TASK;
        destination->SetVacant(true);
      }

      bool Train::Task::PathIdxValid(Train* t)
      {
        int line_idx = t->GetLineIdx();
        if (path.size() == 1)
          return true;
        Logger& l = Logger::GetInstance();
        if (path[path_idx].first->GetIdx() != line_idx)
        {
          for (size_t i = 0; i < path.size(); ++i)
          {
            Edge* e = path[i].first;
            if (e != NULL && e->GetIdx() == line_idx)
            {
              path_idx = (int)i;
              t->SetDirection(path[i].second ? 1 : -1);
              return true;
            }
          }
          return false;
        }
        return true;
      }
      void Train::Task::ChangeCurrentPath(std::pair<Edge*, bool> new_path)
      {
        Logger& l = Logger::GetInstance();
        path[path_idx] = new_path;
        l << std::string("path_idx: ") << std::to_string(path_idx) << std::string("\n");
        for (size_t i = 0; i < path.size(); ++i)
        {
          if (path[i].first != NULL)
            l << std::to_string(path[i].first->GetIdx()) << std::string(" ") << std::to_string(path[i].second) << std::string(", ");
          else
            l << std::string("Empty, ");
          l << std::string("\n");
        }
      }
      bool Train::Task::IsFirst(int position)
      {
        return (path_idx == 1 && path[path_idx].second ? position == path[path_idx].first->GetLength() : position == 0);
      }
      int Train::GetDecCooldown()
      {
        if (cooldown > 0)
          return cooldown--;
        return 0;

      }
      void Train::ResetCooldown()
      {
        switch (level)
        {
        case 1:
        {
          cooldown = 2;
          break;
        }
        case 2:
        {
          cooldown = 1;
          break;
        }
        case 3:
        {
          cooldown = 0;
          break;
        }
        }
      }
      bool Train::IncStopCount()
      {
        if (++stop_count > 2)
          return true;
        return false;
          
      }
}
  }
}
