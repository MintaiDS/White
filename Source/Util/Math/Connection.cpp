#ifndef __unix__
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#define _CRT_SECURE_NO_WARNINGS

#include "Connection.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <thread>

#include <cassert>

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

      void Connection::ReadWhatLeft()
      {
        Logger& l = Logger::GetInstance();
        for (int i = 0; i < need_prev; ++i)
        {
          clock_t start_time = clock();
          char resp[5];
          int retVal = recv(clientSocket, resp, 4, 0);
          if (retVal == SOCKET_ERROR)
          {
            l << std::string("Unable to recv\n");
#ifndef __unix__
            WSACleanup();
#endif
            return;
          }

          int data_left = toInt(resp);
          int data_size = data_left;
          char* data = new char[data_left + 1];
          char* data_buf = data;
          while (data_left)
          {

            retVal = recv(clientSocket, data_buf, data_left, 0);
            if (retVal == SOCKET_ERROR)
            {
              l << std::string("Unable to recv\n");
#ifndef __unix__
              WSACleanup();
#endif
              return;
            }
            data_left -= retVal;
            data_buf = data_buf + retVal;
          }
          data[data_size] = '\0';
          //l << std::string("Got the response\n");
          l << std::string("Finished!\n");
          clock_t end_time = clock();
          l << std::string("Left Recv message time: ");
          l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        }
        need_prev = 0;
      }

      bool Connection::Request(const ActionMessage & msg, ResponseMessage& resp)
      {
        //bool is_move = (msg.actionCode == MOVE);
        //FD_SET ReadSet;

        Logger& l = Logger::GetInstance();
        //l << std::string("Message type ") << std::to_string((int)msg.actionCode) << std::string("\n");
        int retVal = 0;
        std::stringstream ss;
        ss << std::hex;
        toLittleEndian(ss, msg.actionCode);
        toLittleEndian(ss, msg.dataLength);
        ss << msg.data;
        std::string s = ss.str();
        const char* pBuf = s.c_str();

        //l << std::string("Sending request from client\n");
        do {
          retVal = send(clientSocket, pBuf, s.size(), 0);

          if (retVal == SOCKET_ERROR)
          {
            l << std::string("Unable to send\n");
#ifndef __unix__
            WSACleanup();
#endif
            return false;
          }
        } while (retVal <= 0);
        if (msg.actionCode == Action::LOGOUT)
          return true;

        //clock_t start_time = clock();

        char szResponse[5];
        do {
          retVal = recv(clientSocket, szResponse, 4, 0);
          if (retVal == SOCKET_ERROR)
          {
            l << std::string("Unable to recv\n");
#ifndef __unix__
            WSACleanup();
#endif
            return false;
          }
        } while (retVal <= 0);
        resp.result = (Result)toInt(szResponse);
        //resp.result = (Result)toInt(code);

        //clock_t end_time = clock();
        //if (is_move)
        //{
          //l << std::string("1 Recv message time: ");
          //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        //}

        //if (is_move)
        //{
        //  //std::thread read_zero(ReadWhatLeft, &clientSocket);
        //  return true;
        //}
          //return true;

        
        /*if (is_move)
        {
          struct timeval tv;
          tv.tv_sec = 0.;
          tv.tv_usec = 10000.0;
          start_time = clock();
          FD_ZERO(&ReadSet);
          FD_SET(clientSocket, &ReadSet);
          int total = select(0, &ReadSet, NULL, NULL, &tv);
          if (!total)
          {
            need_prev++;
            return true;
          }
          end_time = clock();
          l << std::string("2 Recv message time: ");
          l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        }*/

        clock_t start_time = clock();

        retVal = recv(clientSocket, szResponse, 4, 0);
        if (retVal == SOCKET_ERROR)
        {
          l << std::string("Unable to recv\n");
#ifndef __unix__
          WSACleanup();
#endif
          return false;
        }
        //start_time = clock();

        clock_t end_time = clock();
        l << std::string("Recv message time: ");
        l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");

        resp.dataLength = toInt(szResponse);
        int data_left = resp.dataLength;
        //l << std::to_string(data_left) << std::string("\n");
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

        //end_time = clock();
        //if (is_move)
        //{
          //l << std::string("Recv message time: ");
          //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        //}
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
        
        //ReadWhatLeft();
        
        std::string data = MoveMessage(edge_idx, dir, idx);
        ActionMessage msg = FormActionMessage(Action::MOVE, data);
        ResponseMessage resp;
        Request(msg, resp);
        //l << std::to_string((int)resp.result) << std::string("\n");
        if (resp.result != OKEY)
          l << std::string(resp.data) << std::string("\n");
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
