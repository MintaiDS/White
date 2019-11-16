#include "GraphVisualizer.h"
#include "Renderer.h"
#include "Rectangle.h"
#include "Ring.h"
#include "Disk.h"
#include "Segment.h"
#include "Common.h"
#include "Grid.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace White {
namespace Engine {
namespace Graphics {

GraphVisualizer::GraphVisualizer(Renderer& renderer) 
        : Game(renderer) {}

void GraphVisualizer::LoadGraph(std::string path) {
    graph.reset(ParseGraphFromJSON(path));
}

void GraphVisualizer::UpdateCamera() {
    Program& program = renderer.GetProgram();
    Matrix<GLfloat> view = Matrix<GLfloat>::Identity(2);
    view[0][0] *= (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
    view[1][1] *= (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
    view.Inverse();
    view = {{view[0][0], view[0][1], 0.0f, 0.0f}, 
            {view[1][0], view[1][1], 0.0f, 0.0f}, 
            {0.0f, 0.0f, 1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f, 1.0f}};
    std::unique_ptr<GLfloat[]> raw 
        = std::make_unique<GLfloat[]>(view.rows * view.columns);
    program.Use();
    GLint location = glGetUniformLocation(program.id, "view");
    for (int i = 0; i < view.rows; i++) {
        for (int j = 0; j < view.columns; j++) {
            raw.get()[i * view.columns + j] = view[i][j];
        }
    }
    program.Use();
    glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
}

void GraphVisualizer::Play() {
    if (!graph) {
        LoadGraph("test_graphs/20graph.json"); 
        int verticesCnt = graph->GetVerticesCnt();
        int dimension = std::sqrt(verticesCnt) + 1;
        grid.reset(new Grid({0.0f, 0.0f}, 
                            {dimension, dimension}, 
                            {0.25f, 0.25f}));
        UpdateCamera();
        graphView.SetRenderer(&renderer);
        graphView.SetGraph(graph);
        graphView.SetGrid(grid);
        graphView.Init();
        graphView.Display();
    }
}

}
}
}
