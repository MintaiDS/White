#include "Edge.h"

int White::Util::Math::Edge::GetPointIdxFromPosition(int position)
{
  if (position == 0)
    return v[0];
  if (position == length)
    return v[1];
  return -1;
}
