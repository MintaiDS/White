#include "Overseer.h"
#include <cassert>
#include "Logger.h"
#include <algorithm>
#include <math.h>

#include <time.h>

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

      void Overseer::Init(std::string playerName, std::string game, std::string num_players, std::string num_turns)
      {
        Logger& l = Logger::GetInstance();
        std::string data;
        data = "{\"name\":\"" + playerName + "\"";
        if (game != playerName)
        {
          game_name = game;
          data += ",\"game\":\"" + game + "\"";
        }
        else
        {
          game_name = "Game of " + game;
        }
        if (num_players != playerName)
        {
          l << std::string("Multiplayer!\n");
          data += ",\"num_players\":" + num_players;
        }
        if (num_turns != playerName)
        {
          data += ",\"num_turns\":" + num_turns;
        }
        data += "}";
        l << data << std::string("\n");
        ActionMessage msg = conn.FormActionMessage(Action::LOGIN, data);
        ResponseMessage resp;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseLogin(resp.data);
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
          l << std::string(resp.data) << std::string("\n");
          delete[](resp.data);

        }
        else
          assert(0 && "MAP1 parse");
        GetMyTrains();
        FindMyCity();
        graph->InitWorldPaths(my_city->GetPointIdx());
      }

      bool Overseer::Turn()
      {
        Logger& l = Logger::GetInstance();
        l << std::string("Turn ") <<std::to_string(++(graph->turn_counter)) << std::string("\n");
        GameState game_state = FINISHED;
        ActionMessage msg = conn.FormActionMessage(Action::GAMES, "");
        ResponseMessage resp;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          //l << std::string(resp.data) << std::string("\n");
          game_state = ParseGameState(resp.data);
        }
        delete[](resp.data);
        while (game_state == INIT)
        {
          Sleep(sleep_time);
          conn.Request(msg, resp);
          if (resp.result == Result::OKEY)
            game_state = ParseGameState(resp.data);
          else
            game_state = FINISHED;
        }
        if (game_state == FINISHED)
          return 0;
        assert(game_state == RUN);
        //clock_t start_time = clock();
        
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          UpdateInfrastructureFromJSON(graph, resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "MAP1 update");
        delete[](msg.data);
        
        //clock_t end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        
        l << std::string("Food: ");
        l << my_city->GetCurProduct();
        l << std::string("Armor: ");
        l << my_city->GetCurArmor();
        l << std::string("Population: ");
        l << my_city->GetPopulation();

        //start_time = clock();

        CheckStatus();

        //end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

        //start_time = clock();

        TryUpgrade();

       // end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

        //start_time = clock();

        AssignTasks();

        //end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

        //start_time = clock();

        MakeMoves();

        //end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

        //start_time = clock();

        msg = conn.FormActionMessage(Action::TURN, conn.END_TURN);
        conn.Request(msg, resp);
        while (resp.result != Result::OKEY)
        {
          Sleep(sleep_time);
          conn.Request(msg, resp);
        }

        //end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

        l << std::string("End Turn!\n");
        return 1;
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
          if (graph->GetPointIdxByPosition(t->GetLineIdx(), t->GetPosition()) == my_city->GetPointIdx())
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
        }
        if (city_idx != -1 || idxs.size() > 0)
        {
          l << std::string("Upgrade ");
          if (city_idx != -1)
            l << std::string("City ");
          if (idxs.size() > 0)
          {
            l << std::string("Train ");
            for (int i = 0; i < idxs.size(); ++i)
              l << std::to_string(idxs[i]) << std::string(" level ") << std::to_string(graph->GetTrainByIdx(idxs[i])->GetLevel()) << std::string("\n");
          }
          l << std::string("\n");
          std::string data = conn.UpgradeMessage(idxs, city_idx);
          l << data << std::string("\n");
          auto msg = conn.FormActionMessage(Action::UPGRADE, data);
          ResponseMessage resp;
          conn.Request(msg, resp);
          if (resp.result != OKEY)
            l << std::to_string((int)resp.result) << std::string("\n");
        }
      }

      void Overseer::AssignTasks()
      {
        Logger& l = Logger::GetInstance();
        for (auto t : trains)
        {
          if (t->GetCooldown() == 0)
          {
            /*if ((t->GetTask() != Train::Task::NO_TASK) && (t->task.TaskEnded(t->GetLineIdx(), t->GetPosition())))
            {
              graph->UnblockLine(t->GetLineIdx(), t);
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
                  l << std::to_string(t->GetIdx()) << std::string(" ") << std::to_string((int)(Train::Task::COME_HOME)) << std::string("\n");
                  l << t->GetGoods();
                }
                else
                {
                  auto path = graph->GetPath(point_idx, task.second);
                  t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), point_idx);
                  l << std::to_string(t->GetIdx()) << std::string(" ") << std::to_string((int)(task.first)) << std::string("\n");
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
                l << std::to_string(t->GetIdx()) << std::string(" ") << std::to_string((int)(task.first)) << std::string("\n");
              }
            }*/
            if ((t->GetTask() != Train::Task::NO_TASK) && (t->task.TaskEnded(t->GetLineIdx(), t->GetPosition())))
            {
              graph->UnblockLine(t->GetLineIdx(), t);
              t->task.DropTask();
              int point_idx = t->task.GetDestination()->GetPointIdx();
              if (point_idx == my_city->GetPointIdx())
              {
                t->SetGoods(0);
                t->SetGoodsType(Train::NONE);
              }
              auto task = ChooseTask(t, point_idx);
              if (task.first == Train::Task::NO_TASK)
              {
                auto path = graph->GetPath(point_idx, my_city->GetPointIdx());
                t->task.SetTask(Train::Task::COME_HOME, path, my_city, point_idx);
                l << t->GetGoods();
              }
              else
              {
                auto path = graph->GetPath(point_idx, task.second);
                t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), point_idx);
                l << std::to_string(t->GetIdx()) << std::string(" ") << std::to_string((int)(task.first)) << std::string("\n");
              }
            }
            if (t->GetTask() == Train::Task::NO_TASK)
            {
              int point_idx = graph->GetPointIdxByPosition(t->GetLineIdx(), t->GetPosition());
              assert(point_idx != -1);
              auto task = ChooseTask(t, point_idx);
              if (task.first != Train::Task::NO_TASK)
              {
                auto path = graph->GetPath(my_city->GetPointIdx(), task.second);
                t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), my_city->GetPointIdx());
                l << std::to_string(t->GetIdx()) << std::string(" ") << std::to_string((int)(task.first)) << std::string("\n");
              }
            }
          }
        }
      }

      void Overseer::MakeMoves()
      {
        Logger& l = Logger::GetInstance();
        for (auto t : trains)
        {

          

          if (t->GetDecCooldown() == 0)
          {

            //clock_t start_time = clock();

            if (!t->IsMoved())
              TryMakeMove(t);

            //clock_t end_time = clock();
            //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

            auto move = t->GetMove();

            //start_time = clock();

            l << std::string("Move:") << std::to_string(move.first) << std::string(" ") << std::to_string(move.second) << std::string("\n");
            conn.SendMoveMessage(move.first, move.second, t->GetIdx());

            //end_time = clock();
            //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
          }
          else
          {
            l << std::string("Cooldown ") << std::to_string(t->GetIdx()) << std::string("\n");
            t->SetMoved(true);
          }
        }
      }

      bool Overseer::IsCityCollision(Train* t, Train* tr, std::pair<int, int> step)
      {
        Vertex* v = graph->GetVByIdx(graph->GetPointIdxByPosition(t->GetLineIdx(), t->GetPosition()));
        Post* p = v->GetPost();
        //quite messy check
        return (p != NULL && p->GetPostType() == CITY && step.second != tr->GetDirection() && step.first == tr->GetMove().first);
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
                  if (IsCityCollision(t, tr_coll, step))
                  {
                    t->SetMove({ t->GetLineIdx(), 0 });
                    t->SetDirection(0);
                  }
                  else
                  {
                    //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                    graph->UnblockLine(t->GetLineIdx(), t);
                    graph->BlockLine(step.first, t);

                    t->SetMove(step);
                    t->SetDirection(step.second);
                    t->SetPosition(new_pos);
                    t->SetLineIdx(step.first);
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
                    //t->SetMove(step);
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
                  //t->SetMove(step);
                  t->SetDirection(0);
                }
              }
            }
            else
            {
              Train* tr_block = graph->CheckLine(step.first);
              if (tr_block == NULL || tr_block->GetDirection() == step.second)
              {
                //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                graph->UnblockLine(t->GetLineIdx(), t);
                graph->BlockLine(step.first, t);
                
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
            Train* tr_coll = CheckForCollision(t, t->GetLineIdx(), t->GetPosition() + step.second);
            if (tr_coll != NULL)
            {
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
                  graph->BlockLine(edge->GetIdx(), t);
                  graph->UnblockLine(t->GetLineIdx(), t);

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
              graph->BlockLine(edge->GetIdx(), t);
              graph->UnblockLine(t->GetLineIdx(), t);
              
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
        bool is_city = false;
        int pnt_idx = edge->GetPointIdxFromPosition(position);
        if (pnt_idx != -1)
        {
          Vertex* v = graph->GetVByIdx(pnt_idx);
          Post* p = v->GetPost();
          if (p != NULL && p->GetPostType() == CITY)
            is_city = true;
        }
        for (auto tr : trains)
        {
          if (tr != t)
          {
            Edge* tr_edge = graph->GetEdgeByIdx(tr->GetLineIdx());
            int tr_pnt_idx = tr_edge->GetPointIdxFromPosition(tr->GetPosition());
            if (edge == tr_edge && position == tr->GetPosition() || (tr_pnt_idx != -1 && tr_pnt_idx == pnt_idx))
            {
              if (is_city)
              {
                if (!tr->IsMoved())
                {
                  auto step = tr->task.ContinueMovement(graph->GetEdgeByIdx(tr->GetLineIdx()), tr->GetPosition());
                  t->SetMove({ edge_idx, position - t->GetPosition() });
                  if (IsCityCollision(tr, t, step))
                  {
                    tr->SetMoved(true);
                    tr->SetMove({ tr->GetLineIdx(), 0 });
                    tr->SetDirection(0);
                  }
                }
              }
              else
                return tr;
            }
          }
        }
        return NULL;
      }

      void Overseer::Logout()
      {
        ActionMessage msg = conn.FormActionMessage(Action::LOGOUT, "");
        ResponseMessage resp;
        conn.Request(msg, resp);
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
          double pop_percent = my_city->GetPopulation() / 100.;
          double max_val = 0.;
          Market* mrkt = NULL;
          if (percent < 0.45 + (log(pop_percent * 5) + 3.)/ 8.)
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


      void Overseer::ParseLogin(char* data)
      {
        json json_parsed = json::parse(data);
        player_idx = json_parsed["idx"].get<std::string>();
      }
      void Overseer::FindMyCity()
      {
        for (auto& c : graph->GetCities())
        {
          if (c.second->GetPlayerIdx() == player_idx)
          {
            my_city = c.second;
            graph->SetPlayerIdx(my_city->GetPlayerIdx());
            return;
          }
        }
      }
      Overseer::GameState Overseer::ParseGameState(char * data)
      {
        json json_parsed = json::parse(data);
        json games = json_parsed.at("games");
        for (int i = 0; i < games.size(); ++i)
        {
          std::string name = games[i].at("name");
          if (name == game_name)
          {
            return games[i].at("state");
          }
        }
        return FINISHED;
      }
    }
  }
}