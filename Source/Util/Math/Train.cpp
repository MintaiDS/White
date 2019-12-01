#include "Train.h"
#include "Logger.h"
#include "Graph.h"

#include <cassert>
namespace White {
  namespace Util {
    namespace Math {

      void Train::Task::SetTask(TaskType t, std::vector<std::pair<Edge*, bool>>& path, Post* p, int start_idx)
      {
        task = t;
        this->path = path;
        this->start_idx = start_idx;
        go_back = 0;
        path_idx = 0;
        destination = p;
      }

      std::pair<int, int>* Train::Task::ContinueMovement(Edge* edge, int position)
      {
        Logger& l = Logger::GetInstance();
        int line_idx = edge->GetIdx();
        l << std::to_string(line_idx) + " " + std::to_string(position) + "\n";
        assert(task != NO_TASK);
        Edge* e = path[path_idx].first;
        //l << e->GetFrom() << e->GetTo();
        bool is_back = path[path_idx].second;
        l << 1001;
        if (go_back)
        {
          //add waiting for another train
          if ((is_back && position == e->GetLength()) || (!is_back && position == 0))
          {
            if (path_idx == 0)
              //unaccessable?
              return new std::pair<int, int>{ line_idx, 0 };
            Edge* e_next = path[path_idx - 1].first;
            bool is_next_back = path[path_idx - 1].second;
            if (is_next_back)
              return new std::pair<int, int>{ e_next->GetIdx(), 1 };
            else
              return new std::pair<int, int>{ e_next->GetIdx(), -1 };
          }
        }
        else
        {
          if (position == edge->GetLength() || position == 0)
          {
            int idx;
            if (position == 0)
            {
              idx = edge->GetFrom();
            }
            else
              idx = edge->GetTo();
            l << idx << start_idx;
            if (idx == start_idx)
              return new std::pair<int, int>{ e->GetIdx(), path[path_idx].second ? -1 : 1 };
          }
          //add waiting for another train
          if ((is_back && position == 0) || (!is_back && position == e->GetLength()))
          {
            if (path_idx == path.size() - 1)
            {
              go_back = 1;
              return new std::pair<int, int>{ line_idx, is_back ? 1 : -1 };
            }
            Edge* e_next = path[path_idx + 1].first;
            bool is_next_back = path[path_idx + 1].second;
            l << 1003;
            if (is_next_back)
              return new std::pair<int, int>{ e_next->GetIdx(), -1 };
            else
              return new std::pair<int, int>{ e_next->GetIdx(), 1 };
          }
        }
        l << 1004;
        return NULL;
      }
      bool Train::Task::TaskEnded(int line_idx, int position)
      {
        if (go_back && path_idx == 0)
        {
          if (path[path_idx].second)
            return position == path[path_idx].first->GetLength();
          else
            return position == 0;
        }
        return false;
      }

      void Train::Task::CheckPathIdx(int line_idx)
      {
        if (path[path_idx].first->GetIdx() != line_idx)
        {
          for (int i = 0; i < path.size(); ++i)
          {
            Edge* e = path[i].first;
            if (e->GetIdx() == line_idx)
              path_idx = i;
            break;
          }
        }
      }
    }
  }
}
