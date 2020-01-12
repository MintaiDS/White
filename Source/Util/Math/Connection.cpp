#ifndef __unix__
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#define _CRT_SECURE_NO_WARNINGS

#include "Connection.h"
#include <sstream>
#include <iomanip>
#include <string>


namespace White {
  namespace Util {
    namespace Math {


      void toLittleEndian(std::stringstream& ss, uint32_t n)
      {
        char* str = (char*)&n;
        ss << str[0];
        ss << str[1];
        ss << str[2];
        ss << str[3];
      }

      int toInt(const char* str)
      {
        int* n = (int*)str;
        return *n;
      }

      Connection::~Connection()
      {
        if (clientSocket != SOCKET_ERROR)
        {
#ifdef __unix__
          close(clientSocket)
#else
          closesocket(clientSocket);
          WSACleanup();
#endif
        }
      }

      bool Connection::Connect(const char * hostname, int port)
      {
        WORD ver = MAKEWORD(2, 2);
        WSADATA wsaData;
        int retVal = 0;

#ifndef __unix__
        WSAStartup(ver, (LPWSADATA)&wsaData);
#endif

        clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (clientSocket == SOCKET_ERROR)
        {
          printf("Unable to create socket\n");
#ifndef __unix__
          WSACleanup();
#endif
          return false;
        }

        SOCKADDR_IN serverInfo;

        LPHOSTENT hostEnt;

        hostEnt = gethostbyname(hostname);

        if (!hostEnt)
        {
          printf("Unable to collect gethostbyname\n");
#ifndef __unix__
          WSACleanup();
#endif
          return 1;
        }

        serverInfo.sin_family = PF_INET;
        serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
        serverInfo.sin_port = htons(port);

        retVal = connect(clientSocket, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
        if (retVal == SOCKET_ERROR)
        {
          printf("Unable to connect\n");
#ifndef __unix__
          WSACleanup();
#endif
          return false;
        }
        printf("Connection established\n");
        return true;
      }

      bool Connection::Request(const ActionMessage & msg, ResponseMessage& resp)
      {
        Logger& l = Logger::GetInstance();
        int retVal = 0;
        std::stringstream ss;
        ss << std::hex;
        toLittleEndian(ss, msg.actionCode);
        toLittleEndian(ss, msg.dataLength);
        ss << msg.data;
        std::string s = ss.str();
        const char* pBuf = s.c_str();

        //l << std::string("Sending request from client\n");
        retVal = send(clientSocket, pBuf, s.size(), 0);

        if (retVal == SOCKET_ERROR)
        {
          l << std::string("Unable to send\n");
#ifndef __unix__
          WSACleanup();
#endif
          return false;
        }

        if (msg.actionCode == Action::LOGOUT)
          return true;

        char szResponse[5];
        retVal = recv(clientSocket, szResponse, 4, 0);
        if (retVal == SOCKET_ERROR)
        {
          l << std::string("Unable to recv\n");
#ifndef __unix__
          WSACleanup();
#endif
          return false;
        }
        resp.result = (Result)toInt(szResponse);
        //if (resp.result != Result::OKEY)
          //return false;
        retVal = recv(clientSocket, szResponse, 4, 0);
        if (retVal == SOCKET_ERROR)
        {
          l << std::string("Unable to recv\n");
#ifndef __unix__
          WSACleanup();
#endif
          return false;
        }
        resp.dataLength = toInt(szResponse);
        int data_left = resp.dataLength;
        resp.data = new char[resp.dataLength + 1];
        char* data_buf = resp.data;
        while (data_left)
        {

          retVal = recv(clientSocket, data_buf, data_left, 0);
          if (retVal == SOCKET_ERROR)
          {

            l << std::string("Unable to recv\n");
#ifndef __unix__
            WSACleanup();
#endif
            return false;
          }
          data_left -= retVal;
          data_buf = data_buf + retVal;
        }
        resp.data[resp.dataLength] = '\0';
        //l << std::string("Got the response\n");
        return true;
      }

      ActionMessage Connection::FormActionMessage(Action a, std::string data)
      {
        ActionMessage msg;
        msg.actionCode = a;
        msg.dataLength = data.size();
        msg.data = new char[msg.dataLength + 1];
        strcpy(msg.data, data.c_str());
        return msg;
      }

      void Connection::SendMoveMessage(int edge_idx, int dir, int idx)
      {
        Logger& l = Logger::GetInstance();
        std::string data = MoveMessage(edge_idx, dir, idx);
        ActionMessage msg = FormActionMessage(Action::MOVE, data);
        ResponseMessage resp;
        Request(msg, resp);
        if (resp.result != OKEY)
          l << resp.data << std::string("\n");
        //if (edge_idx == 149)
          //l << std::to_string((int)resp.result);
      }

      std::string Connection::MoveMessage(int lineIdx, int speed, int trainIdx)
      {
        return "{\"line_idx\":" + std::to_string(lineIdx) + ","
          + "\"speed\":" + std::to_string(speed) + ","
          + "\"train_idx\":" + std::to_string(trainIdx) + "}";
      }
      std::string Connection::UpgradeMessage(std::vector<int>& train_idxs, int city_idx)
      {
        std::string s = "{\"posts\":[";
        if (city_idx != -1)
          s += std::to_string(city_idx);
        s += "],\"trains\":[";
        size_t sz = train_idxs.size();
        for (size_t i = 0; i < sz; ++i)
        {
          s += std::to_string(train_idxs[i]);
          if (i != sz - 1)
            s += ",";
        }
        s += "]}";
        return s;
      }
    }
  }
}
