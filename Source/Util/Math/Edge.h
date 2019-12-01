#pragma once

namespace White {
  namespace Util {
    namespace Math {

      class Edge {
      public:
        Edge() {}
        Edge(int idx, int length, int from, int to) : idx(idx), length(length)
        {
          v[0] = from;
          v[1] = to;
        }
        int GetIdx() { return idx; }
        int GetFrom() { return v[0]; }
        int GetTo() { return v[1]; }
        int GetOtherV(int idx) { return idx == v[0] ? v[1] : v[0]; }
        int GetLength() { return length; }

      private:
        int idx;
        int length;
        int v[2];
      };
    }
  }
}