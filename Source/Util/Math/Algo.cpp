#include "Algo.h"
#include "Logger.h"

namespace White {
  namespace Util {
    namespace Math {

      std::vector<std::pair<Edge*, bool>> Dijkstra(Graph & g, int id, const std::map<int, int>* forbidden_e, std::map<int, int>* forbidden_v)
      {
        //Logger& l = Logger::GetInstance();
        //l << 1000;
        std::vector<Vertex*> vrtx = g.GetVertices();
        size_t num_v = vrtx.size();
        std::vector<int> dist(num_v, INT_MAX);
        std::vector<std::vector<int>> edg(num_v);
        std::vector<std::pair<Edge*, bool>> p(num_v);

        std::priority_queue<std::pair<int, int>> q;
        int idx = vrtx[id]->GetIdx();
        dist[id] = 0;
        q.push(std::make_pair(0, idx));
        while (!q.empty()) {
          int v = q.top().second;
          int cur_d = -q.top().first;
          q.pop();
          size_t v_id = g.GetIdByIdx(v);
          if (cur_d > dist[v_id])
            continue;
          std::vector<Edge*> edges = vrtx[v_id]->GetEdgeList();
          for (size_t j = 0; j < edges.size(); ++j) {
            Edge* e = edges[j];
            if (forbidden_e == NULL || forbidden_e->find(e->GetIdx()) == forbidden_e->end())
            {
              int to = e->GetOtherV(v);
              if (forbidden_v == NULL || forbidden_v->find(to) == forbidden_v->end())
              {
                int len = e->GetLength();
                size_t to_id = g.GetIdByIdx(to);
                if (dist[v_id] + len < dist[to_id]) {
                  dist[to_id] = dist[v_id] + len;
                  p[to_id] = { e, to == e->GetTo() ? false : true };
                  q.push(std::make_pair(-dist[to_id], to));
                }
              }
            }
          }
        }
        return p;
      }

      Pnt dist(Vertex * v1, Vertex * v2)
      {
        //Logger& lg = Logger::GetInstance();
        Pnt p1 = v1->GetCoord();
        Pnt p2 = v2->GetCoord();
        return { p2.x - p1.x, p2.y - p1.y };
      }

      void PlaceVertices(Graph & g)
      {
        //Logger& lg = Logger::GetInstance();
        //lg.Init("run.log");
        std::vector<Vertex*> vs = g.GetVertices();
        int sz = vs.size();
        int step = sqrt(sz) + 1;
        for (int i = 0; i < sz; ++i)
        {
          vs[i]->SetCoord(100. + 10. * (i % step), 100. + 10. * (i / step));
        }
        double xvel = 0.;
        double yvel = 0.;
        bool shifted = true;
        while (shifted)
        {
          shifted = false;
          for (int i = 0; i < vs.size(); ++i) {
            Vertex* v0 = vs[i];
            for (int j = 0; j < vs.size(); ++j)
            {
              Vertex* v1 = vs[j];
              Pnt dst = dist(v0, v1);
              double dx = dst.x;
              double dy = dst.y;
              double l = 2.0 * (dx * dx + dy * dy);
              if (l > 0) {
                xvel += (dx * 150.0) / l;
                yvel += (dy * 150.0) / l;
              }
            }
            double weight = (g.GetEdgeCnt() + 1) * 10;
            for (Edge* e : v0->GetEdgeList()) {
              Pnt dst;
              dst = dist(v0, g.GetVByIdx(e->GetOtherV(v0->GetIdx())));
              xvel -= dst.x / weight;
              yvel -= dst.y / weight;
            }
            //lg << xvel << yvel;
            if (fabs(xvel) < 0.1 && fabs(yvel) < 0.1)
              xvel = yvel = 0;
            else
            {
              shifted = true;
              Pnt pnt{ xvel, yvel };
              v0->ShiftCoord(pnt);
            }
          }
        }
      }
    }
  }
}