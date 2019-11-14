#include "Grid.h"

namespace White {

Grid::Grid(float gridPositionX, float gridPositionY, 
           int gridWidth, int gridHeight, float cellWidth, float cellHeight)
        : Grid({gridPositionX, gridPositionY},
               {gridWidth, gridHeight}, {cellWidth, cellHeight}) {}

Grid::Grid(const Util::Math::Vector<float>& gridPosition, 
           const Util::Math::Vector<int>& gridSize, 
           const Util::Math::Vector<float>& cellSize) 
        : gridPosition(gridPosition)
        , gridSize(gridSize)
        , cellSize(cellSize)
        , cells(gridSize[1], std::vector<Cell>(gridSize[0])) {
    float width = gridSize[0] * cellSize[0];
    float height = gridSize[1] * cellSize[1];
    float leftBorder = -width / 2;
    float topBorder = height / 2;
    for (int i = 0; i < gridSize[1]; i++) {
        for (int j = 0; j < gridSize[0]; j++) {
            Util::Math::Vector<float> vertexPosition = {leftBorder 
                                                  + j * cellSize[0] 
                                                  + cellSize[0] / 2.0f,
                                                  topBorder 
                                                  - i * cellSize[1]
                                                  - cellSize[1] / 2.0f};
            vertexPosition += gridPosition;
            cells[i][j] = Cell({i, j}, vertexPosition);
        }
    }
}

Grid::Grid(const Grid& other) 
        : Grid(other.gridPosition, other.gridSize, other.cellSize) {
}

//Grid& Grid::operator=(const Grid& other) {
//    Grid tmp(other);
//
//    return *this;
//}

}
