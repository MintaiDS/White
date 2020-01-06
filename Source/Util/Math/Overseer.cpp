#include "Overseer.h"
#include <cassert>
#include "Logger.h"
#include <algorithm>

#include "json.hpp"

namespace White {
  namespace Util {
    namespace Math {

      using json = nlohmann::json;

      Overseer::Overseer() : conn(Connection::GetInstance(SERVER_ADDR, SERVER_PORT))
      {
        graph = std::make_shared<Graph>();
        Logger& l = Logger::GetInstance();
        l.Init("run.log");
      }

      void Overseer::Init(std::string playerName, std::string game)
      {
        Logger& l = Logger::GetInstance();
        l << playerName << std::string(" ") << game;
        std::string data;
        if (game == playerName)
          data = "{\"name\":\"" + playerName + "\"}";
        else
        {
          l << std::string("Multiplayer!\n");
          data = "{\"name\":\"" + playerName + "\",\"game\":\"" + game + "\",\"num_players\":2}";
        }
        ActionMessage msg = conn.FormActionMessage(Action::LOGIN, data);
        ResponseMessage resp;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          player_idx = GetPlayerIdxFromJson(resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "LOGIN");
        delete[](msg.data);
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER0);
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseGraphFromJSON(graph, resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "MAP0");
        delete[](msg.data);
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseInfrastructureFromJSON(graph, resp.data);
          l << std::string(resp.data);
          delete[](resp.data);

        }
        else
          assert(0 && "MAP1 parse");
        GetMyTrains();
        FindMyCity();
        graph->InitWorldPaths(my_city->GetPointIdx());
      }

      void Overseer::Turn()
      {
        Logger& l = Logger::GetInstance();
        ActionMessage msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
        ResponseMessage resp;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          UpdateInfrastructureFromJSON(graph, resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "MAP1 update");
        delete[](msg.data);
        CheckStatus();
        TryUpgrade();
        AssignTasks();
        MakeMoves();
        msg = conn.FormActionMessage(Action::TURN, conn.END_TURN);
        conn.Request(msg, resp);
        while (resp.result != Result::OKEY)
        {
          Sleep(500);
          conn.Request(msg, resp);
        }
        l << std::string("End Turn!\n");
      }

      void Overseer::CheckStatus()
      {
        //Logger& l = Logger::GetInstance();
        for (auto& t : trains)
        {
          t->SetMoved(false);
          if (t->GetTask() != Train::Task::NO_TASK)
          {
            if (!t->task.PathIdxValid(t->GetLineIdx()))
              t->SetDirection(0);
            //if (t->task.TaskEnded(t->GetLineIdx(), t->GetPosition()))
            //{
              //Post* p = t->task.GetDestination();
              //if (p->GetPostType() == PostType::MARKET)
                //markets.insert({ p->GetMaxProduct(), (Market*)p });
              //t->task.DropTask();
            //}
          }
        }
      }

      void Overseer::TryUpgrade()
      {
        Logger& l = Logger::GetInstance();
        int city_level = my_city->GetLevel();
        int city_armor = my_city->GetCurArmor();
        int city_idx = -1;
        if (city_level < 3 && city_armor > City::level_cost[city_level - 1] + armor_buffer)
        {
          city_idx = my_city->GetIdx();
          my_city->SetCurArmor(city_armor - City::level_cost[city_level - 1]);
        }
        std::vector<int> idxs;
        for (auto t : trains)
        {
          int train_level = t->GetLevel();
          int city_armor = my_city->GetCurArmor();
          if (train_level < 3 && city_armor > Train::level_cost[train_level - 1] + armor_buffer)
          {
            idxs.push_back(t->GetIdx());
            my_city->SetCurArmor(city_armor - Train::level_cost[train_level - 1]);
            t->SetLevel(train_level + 1);
          }
        }
        if (city_idx != -1 || idxs.size() > 0)
        {
          l << std::string("Update ");
          if (city_idx != -1)
            l << std::string("City ");
          if (idxs.size() > 0)
            l << std::string("Train ");
          l << std::string("\n");
          std::string data = conn.UpgradeMessage(idxs, city_idx);
          auto msg = conn.FormActionMessage(Action::UPGRADE, data);
          ResponseMessage resp;
          conn.Request(msg, resp);
          //DS add actions for failure
        }
      }

      void Overseer::AssignTasks()
      {
        Logger& l = Logger::GetInstance();
        l << std::string("Food: ");
        l << my_city->GetCurProduct();
        l << std::string("Armor: ");
        l << my_city->GetCurArmor();
        for (auto t : trains)
        {
          if ((t->GetTask() != Train::Task::NO_TASK) && (t->task.TaskEnded(t->GetLineIdx(), t->GetPosition())))
          {
            UnblockLine(t->GetLineIdx(), t);
            if (t->GetTask() == Train::Task::COME_HOME)
            {
              t->task.DropTask();
            }
            else
            {
              int point_idx = t->task.GetDestination()->GetPointIdx();
              auto task = ChooseTask(t, point_idx);
              if (task.first == Train::Task::NO_TASK)
              {
                auto path = graph->GetPath(point_idx, my_city->GetPointIdx());
                t->task.DropTask();
                t->task.SetTask(Train::Task::COME_HOME, path, my_city, point_idx);
                l << t->GetGoods();
              }
              else
              {
                auto path = graph->GetPath(point_idx, task.second);
                t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), point_idx);
              }
            }
          }
          if (t->GetTask() == Train::Task::NO_TASK)
          {
            auto task = ChooseTask(t, my_city->GetPointIdx());
            if (task.first != Train::Task::NO_TASK)
            {
              auto path = graph->GetPath(my_city->GetPointIdx(), task.second);
              t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), my_city->GetPointIdx());
            }
          }
        }
      }

      void Overseer::MakeMoves()
      {
        Logger& l = Logger::GetInstance();
        for (auto t : trains)
        {
          if (!t->IsMoved())
            TryMakeMove(t);
          auto move = t->GetMove();
          l << std::string("Move:") << std::to_string(move.first) << std::string(" ") << std::to_string(move.second) << std::string("\n");
          conn.SendMoveMessage(move.first, move.second, t->GetIdx());
        }
      }

      void Overseer::TryMakeMove(Train * t)
      {
        Logger& l = Logger::GetInstance();
        l << t->GetIdx();
        t->SetMoved(true);
        auto task = t->GetTask();
        if (task != Train::Task::NO_TASK && task != Train::Task::DEFENDER)
        {
          auto step = t->task.ContinueMovement(graph->GetEdgeByIdx(t->GetLineIdx()), t->GetPosition());
          l << std::string("Step: ") << std::to_string(step.first) + std::string(" ") + std::to_string(step.second) + std::string("\n");
          l << std::string("Position: ") << std::to_string(t->GetLineIdx()) + std::string(" ") + std::to_string(t->GetPosition()) + std::string("\n");
          if (step.first != t->GetLineIdx() || (t->GetPosition() == 0 || t->GetPosition() == graph->GetEdgeByIdx(step.first)->GetLength()))
          {
            int new_pos = step.second == 1 ? 1 : graph->GetEdgeByIdx(step.first)->GetLength() - 1;
            //l << std::string("Got here!\n");
            Train* tr_coll = CheckForCollision(t, step.first, new_pos);
            if (tr_coll != NULL)
            {
              //l << std::string("Found someone!\n");
              int tr_pos = tr_coll->GetPosition();
              if (!tr_coll->IsMoved())
              {
                TryMakeMove(tr_coll);
                if (tr_pos != tr_coll->GetPosition())
                {
                  //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                  UnblockLine(t->GetLineIdx(), t);
                  BlockLine(step.first, t);
                  
                  t->SetMove(step);
                  t->SetDirection(step.second);
                  t->SetPosition(new_pos);
                  t->SetLineIdx(step.first);

                }
                else
                {
                  int common_point, t_line_idx = t->GetLineIdx(), tr_coll_line_idx = tr_coll->GetLineIdx();
                  if (t_line_idx == tr_coll_line_idx)
                  {
                    Edge* e = graph->GetEdgeByIdx(t_line_idx);
                    common_point = step.second < 0 ? e->GetTo() : e->GetFrom();
                  }
                  else
                    common_point = graph->GetCommonPointIdx(t_line_idx, tr_coll_line_idx);
                  if (step.second == tr_coll->GetDirection() || !MakeEvasionMove(t, common_point, t->GetLineIdx(), tr_coll->GetLineIdx()))
                  {
                    //conn.SendMoveMessage(t->GetLineIdx(), 0, t->GetIdx());
                    t->SetMove({ t->GetLineIdx(), 0 });
                    t->SetDirection(0);
                  }
                  else
                    //TryMakeMove(tr_coll);
                    tr_coll->SetMoved(false);
                }
              }
              else
              {
                int common_point, t_line_idx = t->GetLineIdx(), tr_coll_line_idx = tr_coll->GetLineIdx();
                if (t_line_idx == tr_coll_line_idx)
                {
                  Edge* e = graph->GetEdgeByIdx(t_line_idx);
                  common_point = step.second < 0 ? e->GetTo() : e->GetFrom();
                }
                else
                  common_point = graph->GetCommonPointIdx(t_line_idx, tr_coll_line_idx);
                if (step.second == tr_coll->GetDirection() || !MakeEvasionMove(t, common_point, t->GetLineIdx(), tr_coll->GetLineIdx()))
                {
                  //conn.SendMoveMessage(t->GetLineIdx(), 0, t->GetIdx());
                  t->SetMove({ t->GetLineIdx(), 0 });
                  t->SetDirection(0);
                }
              }
            }
            else
            {
              //l << std::string("Made move\n");
              Train* tr_block = CheckLine(step.first);
              if (tr_block == NULL || tr_block->GetDirection() == step.second)
              {
                //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                UnblockLine(t->GetLineIdx(), t);
                BlockLine(step.first, t);
                
                t->SetMove(step);
                t->SetDirection(step.second);
                t->SetPosition(new_pos);
                t->SetLineIdx(step.first);

              }
              else
              {
                //conn.SendMoveMessage(step.first, 0, t->GetIdx());
                t->SetMove({ t->GetLineIdx(), 0 });
                t->SetDirection(0);
              }
            }
          }
          else
          {
            //l << std::string("Here?\n");
            Train* tr_coll = CheckForCollision(t, t->GetLineIdx(), t->GetPosition() + step.second);
            if (tr_coll != NULL)
            {
              //l << std::string("Collision?\n");
              int tr_pos = tr_coll->GetPosition();
              if (!tr_coll->IsMoved())
              {
                TryMakeMove(tr_coll);
                if (tr_pos != tr_coll->GetPosition())
                {
                  if (t->GetDirection() != step.second)
                  {
                    //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                    t->SetMove(step);
                    t->SetDirection(step.second);
                  }
                  t->SetPosition(t->GetPosition() + step.second);
                }
                else
                {
                  //conn.SendMoveMessage(step.first, 0, t->GetIdx());
                  t->SetMove({ t->GetLineIdx(), 0 });
                  t->SetDirection(0);
                }
              }
              else
              {
                //conn.SendMoveMessage(step.first, 0, t->GetIdx());
                t->SetMove({ t->GetLineIdx(), 0 });
                t->SetDirection(0);
              }
            }
            else
            {
              //l << std::string("moved\n");
              //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
              t->SetMove(step);
              t->SetPosition(t->GetPosition() + step.second);
              t->SetDirection(step.second);
            }
          }
        }
      }

      bool Overseer::MakeEvasionMove(Train * t, int point_idx, int locked_edge_from, int locked_edge_to)
      {
        Logger& l = Logger::GetInstance();
        l << locked_edge_from << locked_edge_to;
        int pos, dir;
        Vertex* v = graph->GetVByIdx(point_idx);
        for (auto edge : v->GetEdgeList())
        {
          if (edge->GetIdx() != locked_edge_from && edge->GetIdx() != locked_edge_to)
          {
            if (edge->GetFrom() == point_idx)
            {
              l << std::string("->\n");
              pos = 1;
              dir = 1;
            }
            else
            {
              assert(edge->GetTo() == point_idx);
              l << std::string("<-\n");
              pos = edge->GetLength() - 1;
              dir = -1;
            }
            Train* tr = CheckForCollision(t, edge->GetIdx(), pos);
            if (tr != NULL)
            {
              int tr_pos = tr->GetPosition();
              if (!tr->IsMoved())
              {
                TryMakeMove(tr);
                if (tr_pos != tr->GetPosition())
                {
                  l << edge->GetIdx();
                  //conn.SendMoveMessage(edge->GetIdx(), dir, t->GetIdx());
                  BlockLine(edge->GetIdx(), t);
                  UnblockLine(t->GetLineIdx(), t);

                  t->SetMove({ edge->GetIdx(), dir });
                  if (t->task.IsFirst(t->GetPosition()))
                  {
                    t->task.SetPathIdx(0);
                    t->task.DestroyStartIdx();
                  }
                  t->task.ChangeCurrentPath({ edge, dir > 0 });
                  t->SetDirection(dir);

                  t->SetLineIdx(edge->GetIdx());
                  t->SetPosition(pos);
                  return true;
                }
              }
            }
            else
            {
              l << edge->GetIdx();
              //conn.SendMoveMessage(edge->GetIdx(), dir, t->GetIdx());
              BlockLine(edge->GetIdx(), t);
              UnblockLine(t->GetLineIdx(), t);
              
              t->SetMove({ edge->GetIdx(), dir });
              if (t->task.IsFirst(t->GetPosition()))
              {
                t->task.SetPathIdx(0);
                t->task.DestroyStartIdx();
              }
              t->task.ChangeCurrentPath({ edge, dir > 0 });
              t->SetDirection(dir);

              t->SetLineIdx(edge->GetIdx());
              t->SetPosition(pos);
              return true;
            }
          }
        }
        return false;
      }

      Train* Overseer::CheckForCollision(Train* t, int edge_idx, int position)
      {
        Logger& l = Logger::GetInstance();
        Edge* edge = graph->GetEdgeByIdx(edge_idx);
        int pnt_idx = edge->GetPointIdxFromPosition(position);

        for (auto tr : trains)
        {
          if (tr != t)
          {
            Edge* tr_edge = graph->GetEdgeByIdx(tr->GetLineIdx());
            int tr_pnt_idx = tr_edge->GetPointIdxFromPosition(tr->GetPosition());
            if ((edge == tr_edge && position == tr->GetPosition()) || (pnt_idx != -1 && pnt_idx != my_city->GetIdx() && pnt_idx == tr_pnt_idx))
              return tr;
          }
        }
        return NULL;
      }

      Train * Overseer::CheckLine(int edge_idx)
      {
        std::map<int, Train*>::iterator it;
        if ((it = blocked_lines.find(edge_idx)) != blocked_lines.end())
          return it->second;
        else
          return NULL;
      }

      void Overseer::BlockLine(int edge_idx, Train * t)
      {
        blocked_lines[edge_idx] = t;
      }

      void Overseer::UnblockLine(int edge_idx, Train* t)
      {
        std::map<int, Train*>::iterator it;
        if ((it = blocked_lines.find(edge_idx)) != blocked_lines.end() && it->second == t)
          blocked_lines.erase(edge_idx);
      }

      void Overseer::GetMyTrains()
      {
        for (auto& p : graph->GetTrains())
        {
          if (p.second->GetPlayerIdx() == player_idx)
          {
            p.second->SetDirection(0);
            trains.push_back(p.second);
          }
        }
      }

      std::pair<Train::Task::TaskType, int> Overseer::ChooseTask(Train * t, int point_idx)
      {
        Logger& l = Logger::GetInstance();
        //l << (int)t->GetGoodsType();
        if (t->GetGoodsType() != Train::Goods::ARMOR)
        {
          double percent = getFoodPercentage();
          double max_val = 0.;
          Market* mrkt = NULL;
          if (percent < 0.5)
          {
            for (auto& v : graph->GetMarkets())
            {
              Market* m = v.second;
              if (m->Vacant())
              {
                int dist = graph->GetPathLen(point_idx, m->GetPointIdx());
                int cap = min(m->GetCurProduct() + dist, m->GetMaxProduct());
                cap = min(cap, t->GetGoodsCap() - t->GetGoods());
                //set a koefficient so the further the market is the less the value will be
                double koef = 1.2;
                double val = (double)cap / pow(dist, koef);
                if (val > max_val)
                {
                  max_val = val;
                  mrkt = m;
                }
              }
            }
          }
          if (mrkt != NULL)
          {
            l << std::string("Food val: ");
            l << max_val;
            if (t->GetGoods() == 0 || max_val > 0.9)
              return std::make_pair(Train::Task::GATHER_FOOD, mrkt->GetPointIdx());
          }
        }
        if (t->GetGoodsType() != Train::Goods::FOOD)
        {
          double max_val = 0.;
          Storage* stor = NULL;
          for (auto& v : graph->GetStorages())
          {
            Storage* s = v.second;
            if (s->Vacant())
            {
              int dist = graph->GetPathLen(point_idx, s->GetPointIdx());
              int cap = min(s->GetCurArmor() + dist, s->GetMaxArmor());
              cap = min(cap, t->GetGoodsCap() - t->GetGoods());
              //set a koefficient so the further the market is the less the value will be
              double koef = 1.2;
              double val = (double)cap / pow(dist, koef);
              if (val > max_val)
              {
                max_val = val;
                stor = s;
              }
            }
          }
          if (stor != NULL)
          {
            l << std::string("Armor val: ");
            l << max_val;
            if (t->GetGoods() == 0 || max_val > 0.9)
              return std::make_pair(Train::Task::GATHER_ARMOR, stor->GetPointIdx());
          }
        }
        return std::make_pair(Train::Task::NO_TASK, -1);
      }


      std::string Overseer::GetPlayerIdxFromJson(char* data)
      {
        json json_parsed = json::parse(data);
        return json_parsed["idx"];
      }
      void Overseer::FindMyCity()
      {
        for (auto& c : graph->GetCities())
        {
          if (c.second->GetPlayerIdx() == player_idx)
          {
            my_city = c.second;
            return;
          }
        }
      }
    }
  }
}