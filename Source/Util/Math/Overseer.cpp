#include "Overseer.h"
#include <cassert>
#include "Logger.h"
#include <algorithm>
#include <math.h>
#include "Algo.h"

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
        //l.Init("run.log");
      }

      void Overseer::Init(std::string playerName, std::string game, std::string num_players, std::string num_turns)
      {
        Logger& l = Logger::GetInstance();
        
        ActionMessage msg = conn.FormActionMessage(Action::GAMES, "");
        ResponseMessage resp;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          l << std::string(resp.data) << std::string("\n");
          delete[](resp.data);
        }

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
        msg = conn.FormActionMessage(Action::LOGIN, data);
        resp;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseLogin(resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "LOGIN");
        delete[](msg.data);
        l << std::string("Login\n");
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER0);
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseGraphFromJSON(resp.data);
          //l << std::string(resp.data) << std::string("\n");
          delete[](resp.data);
        }
        else
          assert(0 && "MAP0");
        delete[](msg.data);
        l << std::string("Layer0\n");
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseInfrastructureFromJSON(resp.data);
          //l << std::string(resp.data) << std::string("\n");
          delete[](resp.data);

        }
        else
          assert(0 && "MAP1 parse");
        l << std::string("Layer1\n");
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER10);
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          ParseCoordFromJSON(resp.data);
          //l << std::string(resp.data) << std::string("\n");
          delete[](resp.data);

        }
        else
          assert(0 && "MAP1 parse");
        l << std::string("Layer2\n");
        //PlaceVertices(graph);
        GetMyTrains();
        FindMyCity();
        graph->InitWorldPaths(my_city->GetPointIdx());
        l << std::string("Dijkstra\n");
      }

      bool Overseer::Turn()
      {
        clock_t start_time = clock();
        Logger& l = Logger::GetInstance();
        l << std::string("Turn ") << std::to_string(++(graph->turn_counter)) << std::string("\n");
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
          UpdateInfrastructureFromJSON(resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "MAP1 update");
        delete[](msg.data);
        l << std::string("Rating: ") << std::to_string(rating) << std::string("\n");
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

        conn.ReadWhatLeft();

        clock_t end_time = clock();
        l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        double full_time = (double)(end_time - prev_clock) / CLOCKS_PER_SEC;
        if (prev_clock == 0. || full_time <= 0.91)
        {
          msg = conn.FormActionMessage(Action::TURN, conn.END_TURN);
          conn.Request(msg, resp);
          while (resp.result != Result::OKEY)
          {
            l << std::string("Waiting...\n");
            Sleep(sleep_time);
            conn.Request(msg, resp);
          }
          end_time = clock();
          l << std::string("Sent End Turn!\n");
        }
        else
        {
          l << std::string("End Turn!\n");
        }
        full_time = (double)(end_time - prev_clock) / CLOCKS_PER_SEC;
        //end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        l << std::to_string(full_time) << std::string("\n");
        prev_clock = end_time;
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
            if (!t->task.PathIdxValid(t))
            {
              auto path = t->task.GetPathPart(1);
              int point_idx = path.second ? path.first->GetTo() : path.first->GetFrom();
              if (point_idx != graph->GetPointIdxByPosition(t->GetLineIdx(), t->GetPosition()))
              {
                t->SetFoodIncome(0.);
                t->task.SetPathIdx(t->task.GetPathLen() - 1);
                int pnt = graph->GetCloserPoint(t->GetLineIdx(), t->GetPosition());
                Edge* e = graph->GetEdgeByIdx(t->GetLineIdx());
                auto p = std::make_pair(graph->GetEdgeByIdx(t->GetLineIdx()), pnt == e->GetFrom());
                t->task.ChangeCurrentPath(p);
              }
            }
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
              t->SetGoodsCap(40 * train_level + 1);
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
            for (size_t i = 0; i < idxs.size(); ++i)
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
            if ((t->GetTask() != Train::Task::NO_TASK) && (t->task.TaskEnded(t->GetLineIdx(), t->GetPosition())))
            {
              graph->UnblockLine(t->GetLineIdx(), t);
              if (t->GetTask() == Train::Task::COME_HOME)
                t->SetFoodIncome(0.);
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
              //assert(point_idx != -1);
              auto task = ChooseTask(t, point_idx);
              if (task.first != Train::Task::NO_TASK)
              {
                auto path = graph->GetPath(point_idx, task.second);
                t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), point_idx);
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

            //clock_t start_time = clock();

            l << std::string("Move:") << std::to_string(move.first) << std::string(" ") << std::to_string(move.second) << std::string("\n");
            if (t->IsMessageNeeded())
            {
              l << std::string("Sent Move!\n");
              conn.SendMoveMessage(move.first, move.second, t->GetIdx());
            }

            //clock_t end_time = clock();
            //l << std::string("Move message time: ");
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
        t->SetNeedMessage(false);
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
            t->SetNeedMessage(true);
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
                    t->SetPrevPosition(t->GetPosition());
                    t->SetPosition(new_pos);
                    t->SetPrevLineIdx(t->GetLineIdx());
                    t->SetLineIdx(step.first);
                  }
                }
                else
                {
                  int common_point, t_line_idx = step.first, tr_coll_line_idx = tr_coll->GetLineIdx();
                  Edge* e = graph->GetEdgeByIdx(tr_coll_line_idx);
                  if (e->GetPointIdxFromPosition(tr_coll->GetPosition()) != -1)
                    tr_coll_line_idx = t_line_idx;
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
                int common_point, t_line_idx = step.first, tr_coll_line_idx = tr_coll->GetLineIdx();
                Edge* e = graph->GetEdgeByIdx(tr_coll_line_idx);
                if (e->GetPointIdxFromPosition(tr_coll->GetPosition()) != -1)
                  tr_coll_line_idx = t_line_idx;
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
              if (tr_block == NULL || tr_block->GetDirection() == step.second || tr_block == t)
              {
                //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                graph->UnblockLine(t->GetLineIdx(), t);
                graph->BlockLine(step.first, t);

                t->SetMove(step);
                t->SetDirection(step.second);
                t->SetPrevPosition(t->GetPosition());
                t->SetPosition(new_pos);
                t->SetPrevLineIdx(t->GetLineIdx());
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
                    t->SetNeedMessage(true);
                  }
                  t->SetPrevPosition(t->GetPosition());
                  t->SetPosition(t->GetPosition() + step.second);
                }
                else
                {
                  //conn.SendMoveMessage(step.first, 0, t->GetIdx());
                  t->SetMove({ t->GetLineIdx(), 0 });
                  t->SetDirection(0);
                  t->SetNeedMessage(true);
                }
              }
              else
              {
                //conn.SendMoveMessage(step.first, 0, t->GetIdx());
                t->SetMove({ t->GetLineIdx(), 0 });
                t->SetDirection(0);
                t->SetNeedMessage(true);
              }
            }
            else
            {
              //l << std::string("moved\n");
              if (t->GetDirection() != step.second)
              {
                //conn.SendMoveMessage(step.first, step.second, t->GetIdx());
                t->SetMove(step);
                t->SetDirection(step.second);
                t->SetNeedMessage(true);
              }
              t->SetPrevPosition(t->GetPosition());
              t->SetPosition(t->GetPosition() + step.second);
            }
          }
        }
      }

      bool Overseer::MakeEvasionMove(Train * t, int point_idx, int locked_edge_from, int locked_edge_to)
      {
        Logger& l = Logger::GetInstance();
        l << locked_edge_from << locked_edge_to;
        l << point_idx;
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
              //assert(edge->GetTo() == point_idx);
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
                  t->SetNeedMessage(true);
                  t->SetPrevLineIdx(t->GetLineIdx());
                  t->SetLineIdx(edge->GetIdx());
                  t->SetPrevPosition(t->GetPosition());
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
              t->SetNeedMessage(true);
              t->SetPrevLineIdx(t->GetLineIdx());
              t->SetLineIdx(edge->GetIdx());
              t->SetPrevPosition(t->GetPosition());
              t->SetPosition(pos);
              return true;
            }
          }
        }
        l << std::string("Nothing");
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
        l << std::string("Food income: ");
        l << CountFoodIncome();
        
        l << std::string("task choosing ") << std::to_string(t->GetIdx()) << std::string("\n");
        //l << (int)t->GetGoodsType();

        if (t->GetGoodsType() != Train::Goods::ARMOR)
        {
          double food_income = CountFoodIncome();
          double percent = GetFoodPercentage();
          double pop_percent = my_city->GetPopulation() / 100.;
          double max_val = 0.;
          double max_pure_val = 0.;
          int min_dist = INT_MAX;
          Market* mrkt = NULL;
          if (food_income < 0.7 || percent < 0.45 + (log(pop_percent * 5) + 3.) / 8.)
          {
            for (auto& v : graph->GetMarkets())
            {
              Market* m = v.second;
              if (m->Vacant())
              {
                auto path = graph->GetPath(point_idx, m->GetPointIdx());
                int dist = max(graph->GetPathLen(point_idx, m->GetPointIdx()) * 2, 1);
                if (path.size() > 1)
                {
                  Edge* e = path[1].first;
                  Train* tr = graph->CheckLine(e->GetIdx());
                  if (tr != NULL && tr != t && tr->GetDirection() == path[1].second)
                  {
                    int pos = tr->GetPosition();
                    int extra_dist = path[1].second ? e->GetLength() - pos : pos;
                    dist += extra_dist;
                  }
                }
                int cap = min(m->GetCurProduct() + dist * m->GetResupply(), m->GetMaxProduct());
                cap = min(cap, t->GetGoodsCap() - t->GetGoods());

                //l << cap;
                //l << dist;
                //set a koefficient so the further the market is the less the value will be
                //double koef = 1.05;
                double pure_val = (double)cap / dist;
                double val = (double)cap / dist;
                if (val > max_val || (val == max_val && dist < min_dist))
                {
                  max_val = val;
                  max_pure_val = pure_val;
                  mrkt = m;
                  min_dist = dist;
                }
              }
            }
          }
          if (mrkt != NULL)
          {
            l << std::string("Food val: ");
            l << max_pure_val;
            if (t->GetGoods() == 0 || max_val >= 1.0)
            {
              if (t->GetGoods() == 0)
                t->SetFoodIncome(max_pure_val);
              return std::make_pair(Train::Task::GATHER_FOOD, mrkt->GetPointIdx());
            }
          }
        }
        if (t->GetGoodsType() != Train::Goods::FOOD)
        {
          double max_val = 0.;
          int min_dist = INT_MAX;
          Storage* stor = NULL;
          for (auto& v : graph->GetStorages())
          {
            Storage* s = v.second;
            if (s->Vacant())
            {
              auto path = graph->GetPath(point_idx, s->GetPointIdx());
              int dist = max(graph->GetPathLen(point_idx, s->GetPointIdx()) * 2, 1);
              if (path.size() > 1)
              {
                Edge* e = path[1].first;
                Train* tr = graph->CheckLine(e->GetIdx());
                if (tr != NULL && tr != t && tr->GetDirection() == path[1].second)
                {
                  int pos = tr->GetPosition();
                  int extra_dist = path[1].second ? e->GetLength() - pos : pos;
                  dist += extra_dist;
                }
              }
              int cap = min(s->GetCurArmor() + dist * s->GetResupply(), s->GetMaxArmor());
              cap = min(cap, t->GetGoodsCap() - t->GetGoods());
              //set a koefficient so the further the market is the less the value will be
              //double koef = 1.2;
              double val = (double)cap / dist;
              if (val > max_val || (val == max_val && dist < min_dist))
              {
                max_val = val;
                stor = s;
                min_dist = dist;
              }
            }
          }
          if (stor != NULL)
          {
            l << std::string("Armor val: ");
            l << max_val;
            if (t->GetGoods() == 0 || max_val >= 1.0)
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
      void Overseer::CalcFoodIncome()
      {
      }
      Overseer::GameState Overseer::ParseGameState(char * data)
      {
        json json_parsed = json::parse(data);
        json games = json_parsed.at("games");
        for (size_t i = 0; i < games.size(); ++i)
        {
          std::string name = games[i].at("name");
          if (name == game_name)
          {
            return games[i].at("state");
          }
        }
        return FINISHED;
      }

      std::shared_ptr<Graph> Overseer::ParseGraphFromJSONFile(std::string filename)
      {
        std::ifstream f_inp;
        f_inp.open(filename, std::ifstream::in);
        std::string result = "", buf;
        while (!f_inp.eof())
        {
          std::getline(f_inp, buf);
          result += buf;
        }
        std::shared_ptr<Graph> g = std::make_shared<Graph>();
        char* data = (char*)result.c_str();
        ParseGraphFromJSON(data);
        return g;
      }

      void Overseer::ParseGraphFromJSON(char* data)
      {
        json json_parsed = json::parse(data);
        int idx = json_parsed.at("idx");
        std::string name = json_parsed.at("name");
        graph->SetIdx(idx);
        graph->SetName(name);
        json v_arr = json_parsed.at("points");
        size_t v_size = v_arr.size();
        for (size_t i = 0; i < v_size; ++i)
        {
          json v_j = v_arr[i];
          int v_idx = v_j.at("idx");
          int v_post_idx = v_j["post_idx"].is_null() ? -1 : v_j.at("post_idx");
          Vertex* v = new Vertex(v_idx, v_post_idx);
          graph->AppendVertex(v);
        }
        json e_arr = json_parsed.at("lines");
        size_t e_size = e_arr.size();
        for (size_t i = 0; i < e_size; ++i)
        {
          json e_j = e_arr[i];
          int e_idx = e_j.at("idx");
          int e_len = e_j.at("length");
          json vertices = e_j.at("points");
          Edge* e = new Edge(e_idx, e_len, vertices[0], vertices[1]);
          graph->AppendEdge(e);
        }
      }

      void Overseer::ParseCoordFromJSON(char* data)
      {
        json json_parsed = json::parse(data);
        json coord_j = json_parsed.at("coordinates");
        int idx = json_parsed.at("idx");
        json size_j = json_parsed.at("size");
        graph->SetSize(size_j[0], size_j[1]);
        for (size_t i = 0; i < coord_j.size(); ++i)
        {
          idx = coord_j[i].at("idx");
          Vertex* v = graph->GetVByIdx(idx);
          assert(v != nullptr);
          v->SetCoord(coord_j[i].at("x"), coord_j[i].at("y"));
          //v->SetSize();
        }
      }

      void Overseer::ParseInfrastructureFromJSON(char* data)
      {
        Logger& l = Logger::GetInstance();
        json json_parsed = json::parse(data);
        json posts_j = json_parsed.at("posts");
        json trains_j = json_parsed.at("trains");
        for (size_t i = 0; i < posts_j.size(); ++i)
        {
          json post_j = posts_j[i];
          int type = post_j.at("type");
          int idx = post_j.at("idx");
          int point_idx = post_j.at("point_idx");
          std::string s;
          switch (type)
          {
          case PostType::CITY:
          {
            if (!post_j["player_idx"].is_null())
              s = post_j["player_idx"].get<std::string>();
            else
              s = "null";
            City* c = new City(idx, point_idx, s);
            c->SetCurProduct(post_j["product"]);
            c->SetMaxProduct(post_j["product_capacity"]);
            c->SetCurArmor(post_j["armor"]);
            c->SetMaxArmor(post_j["armor_capacity"]);
            c->SetLevel(post_j["level"]);
            graph->AppendPost(c);
            break;
          }
          case PostType::MARKET:
          {
            Market* m = new Market(idx, point_idx);
            m->SetCurProduct(post_j["product"]);
            m->SetMaxProduct(post_j["product_capacity"]);
            m->SetResupply(post_j["replenishment"]);
            graph->AppendPost(m);
            break;
          }
          case PostType::STORAGE:
          {
            Storage* st = new Storage(idx, point_idx);
            st->SetCurArmor(post_j["armor"]);
            st->SetMaxArmor(post_j["armor_capacity"]);
            st->SetResupply(post_j["replenishment"]);
            graph->AppendPost(st);
            break;
          }
          }
        }
        for (size_t i = 0; i < trains_j.size(); ++i)
        {
          json train_j = trains_j[i];
          int idx = train_j.at("idx");
          int line_idx = train_j.at("line_idx");
          int position = train_j.at("position");
          std::string s = "null";
          if (!train_j["player_idx"].is_null())
            s = train_j["player_idx"].get<std::string>();
          Train* t = new Train(idx, line_idx, position, s);
          int goods = train_j.at("goods");
          int goods_cap = train_j.at("goods_capacity");
          t->SetGoodsCap(goods_cap);
          t->SetGoods(goods);
          t->SetGoodsType(Train::Goods::NONE);
          t->SetLevel(train_j.at("level"));
          graph->AppendTrain(t);
        }
      }

      void Overseer::UpdateInfrastructureFromJSON(char* data)
      {
        Logger& l = Logger::GetInstance();
        json json_parsed = json::parse(data);
        json posts_j = json_parsed.at("posts");
        json trains_j = json_parsed.at("trains");
        for (size_t i = 0; i < posts_j.size(); ++i)
        {
          json post_j = posts_j[i];
          int type = post_j.at("type");
          int idx = post_j.at("idx");
          std::string s;

          switch (type)
          {
          case PostType::CITY:
          {
            City* c = graph->GetCityByIdx(idx);
            c->SetCurProduct(post_j["product"]);
            c->SetMaxProduct(post_j["product_capacity"]);
            c->SetCurArmor(post_j["armor"]);
            c->SetMaxArmor(post_j["armor_capacity"]);
            c->SetLevel(post_j["level"]);
            c->SetPopulation(post_j["population"]);
            break;
          }
          case PostType::MARKET:
          {
            Market* m = graph->GetMarketByIdx(idx);
            m->SetCurProduct(post_j["product"]);
            break;
          }
          case PostType::STORAGE:
          {
            Storage* st = graph->GetStorageByIdx(idx);
            st->SetCurArmor(post_j["armor"]);
            break;
          }
          }
        }
        l << std::string("trains: ") << std::to_string(trains_j.size()) << std::string("\n");
        for (size_t i = 0; i < trains_j.size(); ++i)
        {
          json train_j = trains_j[i];
          int idx = train_j.at("idx");
          std::string player_idx = train_j.at("player_idx");
          json j_event = train_j.at("events");
          int line_idx = train_j.at("line_idx");
          int position = train_j.at("position");
          auto trs = graph->GetTrains();
          if (trs.find(idx) == trs.end())
          {
            Train* t = new Train(idx, line_idx, position, player_idx);
            graph->AppendTrain(t);
          }
          Train* t = graph->GetTrainByIdx(idx);
          if (player_idx == graph->GetPlayerIdx())
          {
            if (j_event.size() != 0)
            {
              graph->turn_counter = j_event[0]["tick"];
              l << train_j.dump() << "\n";
              CollisionCleanup(graph->GetTrainByIdx(idx), true);
            }
            if (t->GetDirection() != 0)
            {
              if (t->GetPrevLineIdx() == line_idx && t->GetPrevPosition() == position)
              {
                if (t->IncStopCount())
                {
                  l << std::string("Stopped train ") << std::to_string(t->GetIdx()) << std::string("\n");
                  CollisionCleanup(t, false);
                  t->ResetStopCount();
                }
              }
              else
                t->ResetStopCount();
            }
          }
          
          int goods = train_j.at("goods");
          int goods_cap = train_j.at("goods_capacity");
          json j_type = train_j.at("goods_type");
          std::string type;
          if (j_type.is_null())
            type = "null";
          else
            type = std::to_string(j_type.get<int>());
          //l << std::string("Goods type: ");
          //l << type;
          //l << std::string("\n");
          Train::Goods g_type = Train::Goods::NONE;
          if (type == "null")
            g_type = Train::Goods::NONE;
          else if (type == "2")
            g_type = Train::Goods::FOOD;
          else if (type == "3")
            g_type = Train::Goods::ARMOR;
          t->SetLineIdx(line_idx);
          t->SetPosition(position);
          t->SetGoods(goods);
          t->SetGoodsCap(goods_cap);
          t->SetGoodsType(g_type);
          t->SetLevel(train_j.at("level"));
        }
        json ratings = json_parsed.at("ratings");
        SetRating(ratings.at(player_idx).at("rating"));
      }

      void Overseer::CollisionCleanup(Train * t, bool need_reset)
      {
        t->task.DropTask();
        t->SetFoodIncome(0.);
        graph->UnblockLine(t->GetLineIdx(), t);
        if (need_reset)
          t->ResetCooldown();
      }
      double Overseer::CountFoodIncome()
      {
        double food_income = -(double)my_city->GetPopulation();
        for (auto t : trains)
          food_income += t->GetFoodIncome();
        return food_income;
      }
    }
  }
}