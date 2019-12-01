#include "Overseer.h"
#include <cassert>
#include "Logger.h"

#include "json.hpp"

namespace White {
  namespace Util {
    namespace Math {

      using json = nlohmann::json;

      Overseer::Overseer() : conn(Connection::GetInstance(SERVER_ADDR, SERVER_PORT))
      {
        graph = std::make_shared<Graph>();
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
            if (t->task.TaskEnded(t->GetLineIdx(), t->GetPosition()))
            {
              t->task.DropTask();
            }
          }
        }
      }

      void Overseer::AssignTasks()
      {
        for (auto& t : trains)
        {
          if (t->GetTask() == Train::Task::NO_TASK)
          {
            if (!markets.empty())
            {
              Market* market = markets.begin()->second;
              markets.erase(markets.begin());
              auto path = graph->GetPath(my_city->GetPointIdx(), market->GetPointIdx());
              t->task.SetTask(Train::Task::GATHER_FOOD, path, market);
            }
          }
        }
      }

      void Overseer::MakeMoves()
      {
        for (auto& t : trains)
        {
          auto task = t->GetTask();
          if (task != Train::Task::NO_TASK && task != Train::Task::DEFENDER)
          {
            auto step = t->task.ContinueMovement(t->GetLineIdx(), t->GetPosition());
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

      void Overseer::SortMarkets()
      {
        for (auto& p : graph->GetMarkets())
        {
          markets.insert({ p.second->GetMaxProduct(), p.second });
        }
      }

      std::string Overseer::GetPlayerIdxFromJson(char* data)
      {
        json json_parsed = json::parse(data);
        return json_parsed["idx"];
      }
    }
  }
}