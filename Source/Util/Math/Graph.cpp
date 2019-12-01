#include "json.hpp"
#include "Graph.h"
#include "Algo.h"
#include <fstream>
//#include "Logger.h"

namespace White {
  namespace Util {
    namespace Math {

      using json = nlohmann::json;

      void Vertex::SetSize(int x, int y)
      {
        coord.x = x;
        coord.y = y;
      }

      void Graph::AppendEdge(Edge* e)
      {
        Vertex* v = GetVByIdx(e->GetFrom());
        v->AppendEdge(e);
        v = GetVByIdx(e->GetTo());
        v->AppendEdge(e);
        edges[e->GetIdx()] = e;
      }

      void Graph::AppendVertex(Vertex* v)
      {
        v->SetId(vertices.size());
        v_ids[v->GetIdx()] = v->GetId();
        vertices.push_back(v);
      }

      void Graph::AppendTrain(Train * t)
      {
        trains[t->GetIdx()] = t;
      }

      void Graph::AppendPost(Post * p)
      {
        int type = p->GetPostType();
        switch (type)
        {
        case PostType::CITY:
          cities[p->GetIdx()] = (City*)p;
          break;
        case PostType::MARKET:
          markets[p->GetIdx()] = (Market*)p;
          break;
        case PostType::STORAGE:
          storages[p->GetIdx()] = (Storage*)p;
          break;
        }
        Vertex* v = GetVByIdx(p->GetPointIdx());
        v->SetPost(p);
      }

      Vertex* Graph::GetVByIdx(int idx)
      {
        size_t id = GetIdByIdx(idx);
        return id != -1 ? vertices[id] : nullptr;
      }

      size_t Graph::GetIdByIdx(int idx)
      {
        std::map<int, size_t>::iterator it;
        if ((it = v_ids.find(idx)) != v_ids.end())
        {
          return it->second;
        }
        return -1;
      }

      void Graph::SetSize(int x, int y)
      {
        map_size.x = x;
        map_size.y = y;
      }

      void Graph::InitWorldPaths()
      {
        world_map.resize(vertices.size());
      }

      void Graph::FillWorldPaths()
      {
        size_t size_v = vertices.size();
        for (int i = 0; i < size_v; ++i)
        {
          world_map[i] = Dijkstra(*this, i, NULL, NULL);
        }
      }

      void Graph::FillWorldPath(int i)
      {
        world_map[i] = Dijkstra(*this, i, NULL, NULL);
      }

      std::vector<std::pair<Edge*, bool>> Graph::GetPath(int i, int j)
      {
        int i_idx = GetIdByIdx(i);
        int j_idx = GetIdByIdx(j);
        if (world_map[i_idx].size() == 0)
          FillWorldPath(i_idx);
        std::vector<std::pair<Edge*, bool>> res;
        while (j_idx != i_idx)
        {
          std::pair<Edge*, bool> edge = world_map[i_idx][j_idx];
          res.push_back(edge);
          j = vertices[j_idx]->GetIdx();
          j_idx = edge.first->GetOtherV(j);
        }
        res.shrink_to_fit();
        size_t sz = res.size();
        for (int i = 0; i < sz / 2; ++i)
        {
          std::swap(res[i], res[sz - 1 - i]);
        }
        return res;
      }

      std::shared_ptr<Graph> ParseGraphFromJSONFile(std::string filename)
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
        ParseGraphFromJSON(g, data);
        return g;
      }

      void ParseGraphFromJSON(std::shared_ptr<Graph> g, char* data)
      {
        //Logger& l = Logger::GetInstance();
        //logger.Init("parse.log");
        json json_parsed = json::parse(data);
        //l << 200;
        //l << std::string(data);
        int idx = json_parsed.at("idx");
        std::string name = json_parsed.at("name");
        //l << 201;
        g->SetIdx(idx);
        //l << 202;
        g->SetName(name);
        //l << 201;
        json v_arr = json_parsed.at("points");
        size_t v_size = v_arr.size();
        for (size_t i = 0; i < v_size; ++i)
        {
          json v_j = v_arr[i];
          int v_idx = v_j.at("idx");
          int v_post_idx = v_j["post_idx"].is_null() ? -1 : v_j.at("post_idx");
          Vertex* v = new Vertex(v_idx, v_post_idx);
          g->AppendVertex(v);
        }
        //l << 202;
        json e_arr = json_parsed.at("lines");
        size_t e_size = e_arr.size();
        for (size_t i = 0; i < e_size; ++i)
        {
          json e_j = e_arr[i];
          int e_idx = e_j.at("idx");
          int e_len = e_j.at("length");
          json vertices = e_j.at("points");
          Edge* e = new Edge(e_idx, e_len, vertices[0], vertices[1]);
          g->AppendEdge(e);
        }
        //l << g->GetVerticesCnt();
      }

      void ParseCoordFromJSON(std::shared_ptr<Graph> g, char* data)
      {
        json json_parsed = json::parse(data);
        json coord_j = json_parsed.at("coordinates");
        //what does this idx mean?
        int idx = json_parsed.at("idx");
        json size_j = json_parsed.at("size");
        g->SetSize(size_j[0], size_j[1]);
        for (int i = 0; i < coord_j.size(); ++i)
        {
          idx = coord_j[i].at("idx");
          Vertex* v = g->GetVByIdx(idx);
          assert(v != nullptr);
          v->SetSize(coord_j[i].at("x"), coord_j[i].at("y"));
        }
      }

      void ParseInfrastructureFromJSON(std::shared_ptr<Graph> g, char* data)
      {
        json json_parsed = json::parse(data);
        json posts_j = json_parsed.at("posts");
        json trains_j = json_parsed.at("trains");
        for (int i = 0; i < posts_j.size(); ++i)
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
            s = post_j["player_idx"].get<std::string>();
            City* c = new City(idx, point_idx, s);
            g->AppendPost(c);
            break;
          }
          case PostType::MARKET:
          {
            Market* m = new Market(idx, point_idx);
            m->SetCurProduct(post_j["product"]);
            m->SetMaxProduct(post_j["product_capacity"]);
            g->AppendPost(m);
            break;
          }
          case PostType::STORAGE:
          {
            Storage* st = new Storage(idx, point_idx);
            g->AppendPost(st);
            break;
          }
          }
        }
        for (int i = 0; i < trains_j.size(); ++i)
        {
          json train_j = trains_j[i];
          int idx = train_j.at("idx");
          int line_idx = train_j.at("line_idx");
          int position = train_j.at("position");
          std::string s = train_j.at("player_idx");
          Train* t = new Train(idx, line_idx, position, s);
          g->AppendTrain(t);
        }
      }

      void UpdateInfrastructureFromJSON(std::shared_ptr<Graph> g, char* data)
      {
        json json_parsed = json::parse(data);
        json posts_j = json_parsed.at("posts");
        json trains_j = json_parsed.at("trains");
        for (int i = 0; i < posts_j.size(); ++i)
        {
          json post_j = posts_j[i];
          int type = post_j.at("type");
          int idx = post_j.at("idx");
          std::string s;

          switch (type)
          {
          case PostType::CITY:
          {
            City* c = g->GetCityByIdx(idx);
            break;
          }
          case PostType::MARKET:
          {
            Market* m = g->GetMarketByIdx(idx);
            m->SetCurProduct(post_j["product"]);
            break;
          }
          case PostType::STORAGE:
          {
            Storage* st = g->GetStorageByIdx(idx);
            break;
          }
          }
        }
        for (int i = 0; i < trains_j.size(); ++i)
        {
          json train_j = trains_j[i];
          int idx = train_j.at("idx");
          int line_idx = train_j.at("line_idx");
          int position = train_j.at("position");
          Train* t = g->GetTrainByIdx(idx);
          t->SetLineIdx(line_idx);
          t->SetPosition(position);
        }
      }
    }
  }
}
=======
namespace Util {
namespace Math {

void Vertex::SetSize(int x, int y) {
    coord.x = x;
    coord.y = y;
}

void Graph::AppendEdge(Edge* e) {
    Vertex* v = GetVByIdx(e->GetFrom());
    v->AppendEdge(e);
    v = GetVByIdx(e->GetTo());
    v->AppendEdge(e);
    e->SetId(edges.size());
    edges.push_back(e);
}

void Graph::AppendVertex(Vertex* v) {
    v->SetId(vertices.size());
    v_ids[v->GetIdx()] = v->GetId();
    vertices.push_back(v);
}

void Graph::AppendTrain(Train * t) {
    trains.push_back(t);
}

void Graph::AppendPost(Post * p) {
    int type = p->GetPostType();
    switch (type) {
    case PostType::CITY:
        cities.push_back((City*)p);
        break;
    case PostType::MARKET:
        markets.push_back((Market*)p);
        break;
    case PostType::STORAGE:
        storages.push_back((Storage*)p);
        break;
    }
    Vertex* v = GetVByIdx(p->GetPointIdx());
    v->SetPost(p);
}

Vertex* Graph::GetVByIdx(int idx) {
    std::map<int, size_t>::iterator it;
    if ((it = v_ids.find(idx)) != v_ids.end()) {
        return vertices[it->second];
    }

    return nullptr;
}

Vertex* Graph::GetVById(size_t id) {
    for (auto it : vertices) {
        if (it->GetId() == id) {
            return it;
        }
    }

    return nullptr; 
}

Train* Graph::GetTrainById(size_t id) {
    return trains[id];
}

Edge* Graph::GetEdgeById(size_t id) {
    for (auto it : edges) {
        if (it->GetId() == id) {
            return it;
        }
    }

    return nullptr; 
}

Edge* Graph::GetEdgeByIdx(int idx) {
    for (auto it : edges) {
        if (it->GetIdx() == idx) {
            return it;
        }
    }

    return nullptr; 
}

size_t Graph::GetVerticesCnt() const {
    return vertices.size();
}

size_t Graph::GetEdgesCnt() const {
    return edges.size();
}

size_t Graph::GetTrainsCnt() const {
    return trains.size();
}

size_t Graph::GetMarketsCnt() const {
    return markets.size();
}

size_t Graph::GetCitiesCnt() const {
    return cities.size();
}

size_t Graph::GetStoragesCnt() const {
    return storages.size();
}

void Graph::SetSize(int x, int y) {
    map_size.x = x;
    map_size.y = y;
}

Graph* ParseGraphFromJSONFile(std::string filename) {
    std::ifstream f_inp;
    f_inp.open(filename, std::ifstream::in);
    std::string result = "", buf;
    while (!f_inp.eof()) {
        std::getline(f_inp, buf);
        result += buf;
    }
    Graph* g = new Graph();
    char* data = (char*)result.c_str();
    ParseGraphFromJSON(*g, data);
    return g;
}

void ParseGraphFromJSON(Graph& g, char* data) {
    json json_parsed = json::parse(data);

    int idx = json_parsed.at("idx");
    std::string name = json_parsed.at("name");
    g.SetIdx(idx);
    g.SetName(name);

    json v_arr = json_parsed.at("points");
    size_t v_size = v_arr.size();
    for (size_t i = 0; i < v_size; ++i) {
        json v_j = v_arr[i];
        int v_idx = v_j.at("idx");
        int v_post_idx = v_j["post_idx"].is_null() ? -1 : v_j.at("post_idx");
        Vertex* v = new Vertex(v_idx, v_post_idx);
        g.AppendVertex(v);
    }

    json e_arr = json_parsed.at("lines");
    size_t e_size = e_arr.size();
    for (size_t i = 0; i < e_size; ++i) {
        json e_j = e_arr[i];
        int e_idx = e_j.at("idx");
        int e_len = e_j.at("length");
        json vertices = e_j.at("points");
        Edge* e = new Edge(e_idx, e_len, vertices[0], vertices[1]);
        g.AppendEdge(e);
    }
}

void ParseCoordFromJSON(Graph & g, char* data) {
    json json_parsed = json::parse(data);
    json coord_j = json_parsed.at("coordinates");
    //what does this idx mean?
    int idx = json_parsed.at("idx");
    json size_j = json_parsed.at("size");
    g.SetSize(size_j[0], size_j[1]);
    for (int i = 0; i < coord_j.size(); ++i) {
        idx = coord_j[i].at("idx");
        Vertex* v = g.GetVByIdx(idx);
        assert(v != nullptr);
        v->SetSize(coord_j[i].at("x"), coord_j[i].at("y"));
    }
}

void ParseInfrastructureFromJSON(Graph & g, char* data) {
    json json_parsed = json::parse(data);
    json posts_j = json_parsed.at("posts");
    json trains_j = json_parsed.at("trains");
    for (int i = 0; i < posts_j.size(); ++i) {
        json post_j = posts_j[i];
        int type = post_j.at("type");
        int idx = post_j.at("idx");
        int point_idx = post_j.at("point_idx");
        std::string s; 
        switch (type) {
        case PostType::CITY: {
          s = post_j["player_idx"].get<std::string>();
          City* c = new City(idx, point_idx, s);
          g.AppendPost(c);
          break;
        }
        case PostType::MARKET: {
          Market* m = new Market(idx, point_idx);
          g.AppendPost(m);
          break;
        }
        case PostType::STORAGE: {
          Storage* st = new Storage(idx, point_idx);
          g.AppendPost(st);
          break;
        }
        }
    }
    for (int i = 0; i < trains_j.size(); ++i) {
        json train_j = trains_j[i];
        int idx = train_j.at("idx");
        int line_idx = train_j.at("line_idx");
        int position = train_j.at("position");
        std::string s = train_j.at("player_idx");
        Train* t = new Train(idx, line_idx, position, s);
        g.AppendTrain(t);
    }
}

}
}
}
