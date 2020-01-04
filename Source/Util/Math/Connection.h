#pragma once
#include <string>
#include <vector>
#ifdef __unix__
#include <unistd>
#define on_Windows 0
#else
#include <winsock.h>
#define on_Windows 1
//#pragma comment(lib, "Ws2_32.lib")
#endif

#include "Logger.h"

namespace White {
  namespace Util {
    namespace Math {

#define SERVER_ADDR "wgforge-srv.wargaming.net"
#define SERVER_PORT 443

      enum Action
      {
        LOGIN = 1,
        LOGOUT = 2,
        MOVE = 3,
        UPGRADE = 4,
        TURN = 5,
        PLAYER = 6,
        GAMES = 7,
        MAP = 10
      };

      enum Result
      {
        OKEY = 0,
        BAD_COMMAND = 1,
        RESOURCE_NOT_FOUND = 2,
        ACCESS_DENIED = 3,
        INAPPROPRIATE_GAME_STATE = 4,
        TIMEOUT = 5,
        INTERNAL_SERVER_ERROR = 500
      };

      struct ActionMessage
      {
        Action actionCode;
        size_t dataLength;
        char* data;
      };

      struct ResponseMessage
      {
        Result result;
        size_t dataLength;
        char* data;
      };

      class Connection
      {
      public:
        const char* LAYER0 = "{\"layer\":0}";
        const char* LAYER1 = "{\"layer\":1}";
        const char* END_TURN = "";
        //const char* LAYER10 = "{\"layer\":10}";

        static Connection& GetInstance(const char* hostname, int port)
        {
          static Connection instance(hostname, port);
          return instance;
        }

        bool Request(const ActionMessage& msg, ResponseMessage& resp);
        ActionMessage FormActionMessage(Action a, std::string data);
        std::string MoveMessage(int lineIdx, int speed, int trainIdx);
        std::string UpgradeMessage(std::vector<int>& train_idxs, int city_idx = -1);
        //bool Reconnect();

      private:
        SOCKET clientSocket = SOCKET_ERROR;

        Connection(const char* hostname, int port) {
          Connect(hostname, port);
        }
        Connection(const Connection&);
        Connection& operator= (Connection&);
        ~Connection();
        bool Connect(const char* hostname, int port);

      };
    }
  }
}

