#include "Train.h"
#include <cassert>
namespace White {
  namespace Util {
    namespace Math {

      void Train::Task::SetTask(TaskType t, std::vector<std::pair<Edge*, bool>>& path, Post* p)
      {
        task = t;
        this->path = path;
        go_back = 0;
        path_idx = 0;
        destination = p;
      }

      std::pair<int, int>* Train::Task::ContinueMovement(int line_idx, int position)
      {
        assert(task != NO_TASK);
        Edge* e = path[path_idx].first;
        bool is_back = path[path_idx].second;
        if (go_back)
        {
          //add waiting for another train
          if ((is_back && position == e->GetLength()) || (!is_back && position == 0))
          {
            if (path_idx == 0)
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
          //add waiting for another train
          if ((is_back && position == 0) || (!is_back && position == e->GetLength()))
          {
            if (path_idx == path.size())
              return new std::pair<int, int>{ line_idx, 0 };
            Edge* e_next = path[path_idx + 1].first;
            bool is_next_back = path[path_idx + 1].second;
            if (is_next_back)
              return new std::pair<int, int>{ e_next->GetIdx(), -1 };
            else
              return new std::pair<int, int>{ e_next->GetIdx(), 1 };
          }
        }
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
            return;
          }
        }
      }
    }
  }
}