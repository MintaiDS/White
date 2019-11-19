#pragma once

#include <vector>
#include <map>
#include <string>

#include "Post.h"
#include "Train.h"

namespace White {
namespace Util {
namespace Math {

struct Pnt {
    int x;
    int y;
};

class Edge {
public:
    Edge() {}
    Edge(int idx, int length, int from, int to) : idx(idx), length(length) {
        v[0] = from;
        v[1] = to;
    }
    int GetIdx() { return idx; }
    int GetFrom() { return v[0]; }
    int GetTo() { return v[1]; }
    int GetLength() { return length; }
    size_t GetId() const { return id; };
    void SetId(size_t id) { this->id = id; }
  
private:
    size_t id;
    int idx;
    int length;
    int v[2];
};

class Vertex {
public:
    Vertex() : post(nullptr) {}
    Vertex(int idx, int post_idx = -1) : post(nullptr), idx(idx), post_idx(post_idx) {}

    int GetIdx() { return idx; }
    size_t GetId() { return id; }
    Post* GetPost() { return post; }

    void SetId(size_t id) { this->id = id; }
    void SetSize(int x, int y);
    void SetPost(Post* post) { this->post = post; }

    void AppendEdge(Edge* e) { edges.push_back(e); }

    std::vector<Edge*>& GetEdgeList() { return edges; }

private:
    size_t id;
    int idx;
    int post_idx;
    std::vector<Edge*> edges;
    Pnt coord;
    Post* post;
};

class Graph {
public:
    Graph() {}
    Graph(int idx, std::string name) : idx(idx), name(name) {}
    
    void AppendEdge(Edge* e);
    void AppendVertex(Vertex* v);
    void AppendTrain(Train* t);
    void AppendPost(Post* p);

    size_t GetVerticesCnt() const;
    size_t GetEdgesCnt() const;
    size_t GetTrainsCnt() const;
    size_t GetMarketsCnt() const;
    size_t GetCitiesCnt() const;
    size_t GetStoragesCnt() const;
    Vertex* GetVByIdx(int idx);
    Vertex* GetVById(size_t id);
    Edge* GetEdgeByIdx(int idx);
    Edge* GetEdgeById(size_t id);
    Train* GetTrainByIdx(int idx);
    Train* GetTrainById(size_t id);
    Storage* GetStorageByIdx(int idx);
    Storage* GetStorageById(size_t id);
    Market* GetMarketByIdx(int idx);
    Market* GetMarketById(size_t id);
    City* GetCityByIdx(int idx);
    City* GetCityById(size_t id);

    std::vector<Vertex*>& GetVertices() { return vertices; }
    std::vector<Edge*>& GetEdges() { return edges; }
    std::vector<Train*>& GetTrains() { return trains; }
    std::vector<Market*>& GetMarkets() { return markets; }
    std::vector<Storage*>& GetStorages() { return storages; }
    std::vector<City*>& GetCities() { return cities; }

    void SetIdx(int idx) { this->idx = idx; }
    void SetName(const std::string& name) { this->name = name; }

    void SetSize(int x, int y);

private:
    int idx;
    std::string name;
    std::vector<Edge*> edges;
    std::vector<Vertex*> vertices;
    std::map<int, size_t> v_ids;
    Pnt map_size;
    std::vector<City*> cities;
    std::vector<Market*> markets;
    std::vector<Storage*> storages;
    std::vector<Train*> trains;
};

Graph* ParseGraphFromJSONFile(std::string filename);
void ParseGraphFromJSON(Graph& g, char* data);
void ParseCoordFromJSON(Graph& g, char* data);
void ParseInfrastructureFromJSON(Graph& g, char* data);

}
}
}
