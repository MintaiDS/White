#include "Cell.h"

namespace White {

Cell::Cell(const Util::Math::Vector<int>& cellPosition,
           const Util::Math::Vector<float>& vertexPosition) 
        : cellPosition(cellPosition)
        , vertexPosition(vertexPosition) {}

}
