#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "Post.h"
#include "Train.h"
#include "Edge.h"

namespace White {
  namespace Util {
    namespace Math {

      class Graph;

      struct Pnt {
        double x;
        double y;
        Pnt& operator += (Pnt& r)
        {
          this->x += r.x;
          this->y += r.y;
          return *this;
        }
      };

      class Path {
      public:
        Path() {};
        Path(std::vector<std::pair<Edge*, bool>> v, int dist) : path(v), distance(dist) {}
        static Path FormPath(Graph& g, int i_idx, int j_idx, std::vector<std::pair<Edge*, bool>>& dijkstra);

        std::vector<std::pair<Edge*, bool>> GetPath() { return path; }
        int GetDist() { return distance; }
      private:
        std::vector<std::pair<Edge*, bool>> path;
        int distance;
      };


      class Vertex {

      public:
        Vertex() {}
        Vertex(int idx, int post_idx = -1) : idx(idx), post_idx(post_idx) {}

        int GetIdx() { return idx; }
        size_t GetId() { return id; }
        Post* GetPost() { return post; }

        void SetId(size_t id) { this->id = id; }
        void SetSize(int x, int y);
        void SetPost(Post* post) { this->post = post; }

        void AppendEdge(Edge* e) { edges.push_back(e); }

        std::vector<Edge*>& GetEdgeList() { return edges; }
        Pnt GetCoord() { return coord; }
        void SetCoord(double x, double y) { coord = { x, y }; }
        void ShiftCoord(Pnt& p) { coord += p; }

      private:
        size_t id;
        int idx;
        int post_idx;
        std::vector<Edge*> edges;
        Pnt coord;
        Post* post = NULL;
      };

      class Graph {
      public:
        Graph() {}
        Graph(int idx, std::string name) : idx(idx), name(name) {}

        void AppendEdge(Edge* e);
        void AppendVertex(Vertex* v);
        void AppendTrain(Train* t);
        void AppendPost(Post* p);

        Vertex* GetVByIdx(int idx);
        size_t GetIdByIdx(int idx);
        Vertex* GetVById(int id) { return vertices[id]; }
        std::vector<Vertex*>& GetVertices() { return vertices; }
        int GetVerticesCnt() { return vertices.size(); }

        void SetIdx(int idx) { this->idx = idx; }
        void SetName(const std::string& name) { this->name = name; }

        void SetSize(int x, int y);

        void InitWorldPaths(int my_city_idx);
        //void FillWorldPaths();
        void FillWorldPath(int idx);
        std::vector<std::pair<Edge*, bool>> GetPath(int i, int j);
        std::vector<std::pair<Edge*, bool>> GetPathReverse(int i, int j);
        int GetPathLen(int i, int j);

        City* GetCityByIdx(int idx) { return cities[idx]; }
        Market* GetMarketByIdx(int idx) { return markets[idx]; }
        Storage* GetStorageByIdx(int idx) { return storages[idx]; }
        Train* GetTrainByIdx(int idx) { return trains[idx]; }
        Edge* GetEdgeByIdx(int idx) { return edges[idx]; }

        std::map<int, Train*>& GetTrains() { return trains; }
        std::map<int, Market*>& GetMarkets() { return markets; }
        std::map<int, Storage*>& GetStorages() { return storages; }
        std::map<int, City*>& GetCities() { return cities; }
        std::map<int, Edge*>& GetEdges() { return edges; }
        int GetEdgeCnt() { return edges.size(); }

        int GetCommonPointIdx(int edge_idx1, int edge_idx2);

      private:
        int idx;
        std::string name;
        std::map<int, Edge*> edges;
        std::vector<Vertex*> vertices;
        std::map<int, size_t> v_ids;
        Pnt map_size;
        std::map<int, City*> cities;
        std::map<int, Market*> markets;
        std::map<int, Storage*> storages;
        std::map<int, Train*> trains;
        std::vector<std::vector<Path*>> world_map;
        //std::vector<std::vector<int>> distance;
      };

      std::shared_ptr<Graph> ParseGraphFromJSONFile(std::string filename);
      void ParseGraphFromJSON(std::shared_ptr<Graph> g, char* data);
      void ParseCoordFromJSON(std::shared_ptr<Graph> g, char* data);
      void ParseInfrastructureFromJSON(std::shared_ptr<Graph> g, char* data);
      void UpdateInfrastructureFromJSON(std::shared_ptr<Graph> g, char* data);

    }
  }
}


