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
        //Logger& l = Logger::GetInstance();
        std::string data = "{\"name\":\"" + playerName + "\"}";
        ActionMessage msg = conn.FormActionMessage(Action::LOGIN, data);
        ResponseMessage resp;
        //l << 20;
        conn.Request(msg, resp);
        if (resp.result == Result::OKEY)
        {
          player_idx = GetPlayerIdxFromJson(resp.data);
          delete[](resp.data);
        }
        else
          assert(0 && "LOGIN");
        //l << 21;
        delete[](msg.data);
        msg = conn.FormActionMessage(Action::MAP, conn.LAYER0);
        conn.Request(msg, resp);
        //l << resp.result;
        if (resp.result == Result::OKEY)
        {
          ParseGraphFromJSON(graph, resp.data);
          //l << 210;
          delete[](resp.data);
        }
        else
          assert(0 && "MAP0");
        //l << 22;
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
        //l << 23;
        GetMyTrains();
        FindMyCity();
        graph->InitWorldPaths();
      }

      void Overseer::Turn()
      {
        //Logger& l = Logger::GetInstance();
        //l << 1;
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
        //l << 2;
        CheckStatus();
        //l << 3;
        AssignTasks();
        //l << 4;
        MakeMoves();
        //l << 5;
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
        //l << 10;
        for (auto& t : trains)
        {
          //l << (int)(t->GetTask());
          if (t->GetTask() != Train::Task::NO_TASK)
          {
            //l << 12;
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

      void Overseer::AssignTasks()
      {
        Logger& l = Logger::GetInstance();
        //l << 0;
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
                //l << std::string("home\n");
                auto path = graph->GetPathReverse(point_idx, my_city->GetPointIdx());
                //l << point_idx << my_city->GetPointIdx();
                //l << path[0].second;
                t->task.DropTask();
                t->task.SetTask(Train::Task::COME_HOME, path, my_city, point_idx);
                l << t->GetGoods();
              }
              else
              {
                auto path = graph->GetPath(point_idx, task.second);
                //l << path.size();
                //l << std::string("market\n");
                //l << my_city->GetPointIdx() << market->GetPointIdx();
                t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), point_idx);
              }
            }
          }
          if (t->GetTask() == Train::Task::NO_TASK)
          {
            //l << 1;
            auto task = ChooseTask(t, my_city->GetPointIdx());
            if (task.first != Train::Task::NO_TASK)
            {
              auto path = graph->GetPath(my_city->GetPointIdx(), task.second);
              //l << path.size();
              //l << std::string("market\n");
              //l << my_city->GetPointIdx() << market->GetPointIdx();
              t->task.SetTask(task.first, path, graph->GetVByIdx(task.second)->GetPost(), my_city->GetPointIdx());
            }
              //l << 6;
            //}
          }
        }
      }

      void Overseer::MakeMoves()
      {
        //Logger& l = Logger::GetInstance();
        //l.Init("run.log");
        for (auto& t : trains)
        {
          auto task = t->GetTask();
          if (task != Train::Task::NO_TASK && task != Train::Task::DEFENDER)
          {
            //l << 10;
            auto step = t->task.ContinueMovement(graph->GetEdgeByIdx(t->GetLineIdx()), t->GetPosition());
            if (step != NULL)
            {
              //l << 20;
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
        if (t->GetGoodsType() != Train::Goods::ARMOR)
        {
          double percent = getFoodPercentage();
          double max_val = 0.;
          Market* mrkt = NULL;
          if (percent < 0.8)
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