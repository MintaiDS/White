#pragma once

#include "Vector.h"
#include "Cell.h"

#include <vector>

namespace White {

struct Grid {
    Grid(float gridPositionX = 0.0f, float gridPositionY = 0.0f,
         int gridWidth = 0, int gridHeight = 0, 
         float cellWidth = 0.0f, float cellHeight = 0.0f);
    Grid(const Util::Math::Vector<float>& gridPosition = {0.0f, 0.0f},
         const Util::Math::Vector<int>& gridSize = {0, 0},
         const Util::Math::Vector<float>& cellSize = {0.0f, 0.0f});
    Grid(const Grid& other);

    Util::Math::Vector<float> gridPosition;
    Util::Math::Vector<int> gridSize;
    Util::Math::Vector<float> cellSize; 
    std::vector<std::vector<Cell>> cells; 
};

}
