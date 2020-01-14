#include "Algo.h"
#include "Logger.h"
#include "Vector.h"

namespace White {
  namespace Util {
    namespace Math {

      std::vector<std::pair<Edge*, bool>> Dijkstra(Graph & g, int idx, const std::map<int, int>* forbidden_e, std::map<int, int>* forbidden_v)
      {
        //Logger& l = Logger::GetInstance();
        std::vector<Vertex*> vrtx = g.GetVertices();
        size_t num_v = vrtx.size();
        std::vector<int> dist(num_v, INT_MAX);
        std::vector<std::vector<int>> edg(num_v);
        std::vector<std::pair<Edge*, bool>> p(num_v);

        std::priority_queue<std::pair<int, int>> q;
        int id = g.GetIdByIdx(idx);
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

      Math::Vector<float> dist(Vertex * v1, Vertex * v2)
      {
        //Logger& lg = Logger::GetInstance();
        auto p1 = v1->GetCoord();
        auto p2 = v2->GetCoord();
        return p2 - p1;
      }

      void PlaceVertices(std::shared_ptr<Graph> g)
      {
        int num = 0;
        Logger& lg = Logger::GetInstance();
        //lg.Init("run.log");
        std::vector<Vertex*> vs = g->GetVertices();
        std::vector<Math::Vector<float>> shift(vs.size());
        int sz = vs.size();
        int step = sqrt(sz) + 1;
        for (int i = 0; i < sz; ++i)
        {
          vs[i]->SetCoord(100. + 10. * (i % step), 100. + 10. * (i / step));
          shift[i] = { 0.0, 0.0 };
        }
        double xvel = 0.;
        double yvel = 0.;
        bool shifted = true;
        while (shifted)
        {
          num++;
          for (int i = 0; i < vs.size(); ++i)
            vs[i]->ShiftCoord(shift[i]);
          shifted = false;
          for (int i = 0; i < vs.size(); ++i) {
            Vertex* v0 = vs[i];
            xvel = 0.;
            yvel = 0.;
            for (int j = 0; j < vs.size(); ++j)
            {
              Vertex* v1 = vs[j];
              auto dst = dist(v1, v0);
              double dx = dst[0];
              double dy = dst[1];
              double l = 4.0 * (dx * dx + dy * dy);
              if (l > 0) {
                xvel += (dx * 150.0) / l;
                yvel += (dy * 150.0) / l;
              }
            }
            //printf("%d %f %f\n", i, xvel, yvel);
            double weight = (v0->GetEdgeList().size() + 1) * 1;
            for (Edge* e : v0->GetEdgeList()) {
              auto dst = dist(g->GetVByIdx(e->GetOtherV(v0->GetIdx())), v0);
              xvel -= dst[0] / weight;
              yvel -= dst[1] / weight;
            }
            if (fabs(xvel) < 0.01 && fabs(yvel) < 0.01)
              xvel = yvel = 0;
            else
            {
              shifted = true;
              //Pnt pnt{ xvel, yvel };
              //v0->ShiftCoord(pnt);
            }
            shift[i][0] = xvel;
            shift[i][1] = yvel;
          }
          //for (int i = 0; i < shift.size(); ++i)
            //printf("%d: %f %f\n", i, shift[i].x, shift[i].y);
          //system("pause");
        }
        lg << std::string("Placed in ") + std::to_string(num) + "\n";
        //system("pause");
      }
    }
  }
}