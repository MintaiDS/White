#pragma once

#include "Vector.h"

namespace White {

struct Cell {
    Cell(const Util::Math::Vector<int>& cellPosition = {0, 0}, 
         const Util::Math::Vector<float>& vertexPosition = {0.0f, 0.0f});

//    Cell& operator=(const Cell& other);

    Util::Math::Vector<int> cellPosition;
    Util::Math::Vector<float> vertexPosition; 
};

}
