#include "GraphView.h"
#include "Disk.h"
#include "Ring.h"
#include "Segment.h"

using namespace White::Util;

GraphView::GraphView() 
        : renderer(nullptr)
        , graph(nullptr)
        , grid(nullptr) {}

void GraphView::Init() {
    for (int i = 0; i < graph->GetVerticesCnt(); i++) {
        int row = i / (grid->gridSize[1]);
        int column = i % (grid->gridSize[1]);
        cells.push_back(grid->cells[row][column]);
    }
}

void GraphView::Display() {
    for (int i = 0; i < graph->GetVerticesCnt(); i++) {
        DisplayNode(i);
    }
    for (int i = 0; i < graph->GetEdgesCnt(); i++) {
        Edge* edge = graph->GetEdgeById(i);
        int from = graph->GetVByIdx(edge->GetFrom())->GetId();
        int to = graph->GetVByIdx(edge->GetTo())->GetId();
        DisplayEdge(from, to);
    }
}

void GraphView::DisplayNode(int node) {
    Math::Vector<float> pos = cells[node].vertexPosition;

    Math::Vector<float> color = {1.0f, 1.0f, 0.0f, 1.0f};
    Math::Disk<float> disk(0.2);
    Mesh<float> diskMesh = disk.ToMesh(color, 720);
    diskMesh.Scale({0.2f, 0.2f, 1.0f});
    diskMesh.Translate({pos[0], pos[1], 0.0f});
    renderer->AddMesh(diskMesh);

    color = {0.0f, 0.0f, 0.5f, 1.0f};
    Math::Ring<float> ring(0.1, 0.28);
    Mesh<float> ringMesh = ring.ToMesh(color, 720);
    ringMesh.Scale({0.2f, 0.2f, 1.0f});
    ringMesh.Translate({pos[0], pos[1], 0.0f}); 
    renderer->AddMesh(ringMesh);
}

void GraphView::DisplayEdge(int from, int to) {
    Math::Vector<float> color = {0.0f, 0.0f, 0.0f, 1.0f};
    Math::Vector<float> begin = cells[from].vertexPosition;
    Math::Vector<float> end = cells[to].vertexPosition; 
    Math::Vector<float> dir = end - begin;
    Math::Vector<float> mid = begin + dir * (1.0 / 2.0f);
    Math::Vector<float> initial = {dir.Length(), 0};
    float cross = initial.Dot(dir);
    float phi = Math::ToDegrees(std::acos(cross / (dir.Length() 
                                          * initial.Length())));
    Math::Vector<float> rotation = {0.0f, 0.0f, -phi};
    Math::Segment<float> segment(begin, end);
    Mesh<float> segmentMesh = segment.ToMesh(color, 4);
    segmentMesh.Translate({mid[0], mid[1], 0.1f});
    segmentMesh.Rotate(rotation);
    renderer->AddMesh(segmentMesh);
}

void GraphView::SetRenderer(White::Engine::Graphics::Renderer* renderer) {
    this->renderer = renderer;
}
void GraphView::SetGraph(Math::Graph* graph) {
    this->graph = graph;
}

void GraphView::SetGrid(White::Grid* grid) {
    this->grid = grid;
}
