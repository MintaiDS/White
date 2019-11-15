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

void GraphVisualizer::Play() {
    if (!graph) {
        LoadGraph("test_graphs/10graph.json"); 
        int verticesCnt = graph->GetVerticesCnt();
        int dimension = std::sqrt(verticesCnt) + 1;
        grid.reset(new Grid({0.0f, 0.0f}, 
                            {dimension, dimension}, 
                            {0.25f, 0.25f}));
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
