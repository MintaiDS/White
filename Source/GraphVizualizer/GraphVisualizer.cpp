#include "GraphVisualizer.h"
#include "Renderer.h"
#include "Rectangle.h"
#include "Ring.h"
#include "Disk.h"
#include "Segment.h"
#include "Common.h"
#include "Grid.h"

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
        LoadGraph("Temp/small_graph.json"); 
        int verticesCnt = graph->GetVerticesCnt();
        grid.reset(new Grid({0.0f, 0.0f}, 
                            {verticesCnt / 2, verticesCnt / 2}, 
                            {0.15f, 0.15f}));
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
