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

      void Overseer::Init(std::string playerName)
      {
        Logger& l = Logger::GetInstance();
        std::string data = "{\"name\":\"" + playerName + "\"}";
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
        if (resp.result == Result::OKEY)
        {
        }
        else
          assert(0 && "END TURN");
      }

      void Overseer::CheckStatus()
      {
        //Logger& l = Logger::GetInstance();
        for (auto& t : trains)
        {
          if (t->GetTask() != Train::Task::NO_TASK)
          {
            t->task.CheckPathIdx(t->GetLineIdx());
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
        //Logger& l = Logger::GetInstance();
        for (auto& t : trains)
        {
          auto task = t->GetTask();
          if (task != Train::Task::NO_TASK && task != Train::Task::DEFENDER)
          {
            auto step = t->task.ContinueMovement(graph->GetEdgeByIdx(t->GetLineIdx()), t->GetPosition());
            if (step != NULL)
            {
              std::string data = conn.MoveMessage(step->first, step->second, t->GetIdx());
              ActionMessage msg = conn.FormActionMessage(Action::MOVE, data);
              ResponseMessage resp;
              conn.Request(msg, resp);
            }
          }
        }
      }

      void Overseer::GetMyTrains()
      {
        for (auto& p : graph->GetTrains())
        {
          if (p.second->GetPlayerIdx() == player_idx)
          {
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