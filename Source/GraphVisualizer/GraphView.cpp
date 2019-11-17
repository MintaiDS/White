#include "GraphView.h"
#include "Disk.h"
#include "Ring.h"
#include "Segment.h"
#include "DigitMeshCreator.h"
#include "CharacterBlock.h"

#include <algorithm>
#include <sstream>

using namespace White::Util;

GraphView::GraphView() 
        : renderer(nullptr) {}

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
        DisplayEdge(i);
    }
}

void GraphView::DisplayNode(int node) {
    Math::Vector<float> pos = cells[node].vertexPosition;

    Math::Vector<float> color = {1.0f, 1.0f, 0.0f, 1.0f};
    Math::Disk<float> disk(0.2);
    Mesh<float> diskMesh = disk.ToMesh(color, 720);
    diskMesh.Scale({0.2f, 0.2f, 1.0f});
    diskMesh.Translate({pos[0], pos[1], 0.3f});
    renderer->AddMesh(diskMesh);

    color = {0.0f, 0.0f, 0.5f, 1.0f};
    Math::Ring<float> ring(0.1, 0.28);
    Mesh<float> ringMesh = ring.ToMesh(color, 720);
    ringMesh.Scale({0.2f, 0.2f, 1.0f});
    ringMesh.Translate({pos[0], pos[1], 0.3f}); 
    renderer->AddMesh(ringMesh);
    
    std::stringstream str;
    str << graph->GetVById(node)->GetIdx();
    White::Engine::Graphics::CharacterBlock charBlock({pos[0], pos[1], 0.1f}, 
                                                      {0.032f, 0.032f}, str.str());
    charBlock.Scale({0.028f, 0.028f, 1.0f});
    charBlock.Translate({-0.008f, 0.0f, 0.1f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        renderer->AddMesh(meshes[i]);
    }
}

void GraphView::DisplayEdge(int edge) {
    Edge* edgePtr = graph->GetEdgeById(edge);
    int from = graph->GetVByIdx(edgePtr->GetFrom())->GetId();
    int to = graph->GetVByIdx(edgePtr->GetTo())->GetId();
    Math::Vector<float> color = {0.7f, 0.7f, 0.6f, 1.0f};
    Math::Vector<float> begin = cells[from].vertexPosition;
    Math::Vector<float> end = cells[to].vertexPosition; 
    Math::Vector<float> dir = end - begin;
    Math::Vector<float> mid = begin + dir * (1.0f / 2.0f);
    Math::Vector<float> initial = {dir.Length(), 0};
    float dot = initial.Dot(dir);
    Math::Vector<float> diff = initial - dir;
    float phi = Math::ToDegrees(atan2(initial[0] * dir[1] - dir[0] * initial[1],
                                      initial[0] * dir[0] + initial[1] * dir[1]));
    Math::Vector<float> rotation = {0.0f, 0.0f, phi};
    Math::Segment<float> segment(begin, end);
    Mesh<float> segmentMesh = segment.ToMesh(color, 4);
    segmentMesh.Translate({mid[0], mid[1], 0.9f});
    segmentMesh.Rotate(rotation);
    renderer->AddMesh(segmentMesh);

    std::stringstream str;
    str << edgePtr->GetLength();
    White::Engine::Graphics::CharacterBlock charBlock({mid[0], mid[1], 0.1f}, 
                                                      {0.032f, 0.032f}, str.str());
    charBlock.Scale({0.028f, 0.028f, 1.0f});
    charBlock.Translate({-0.008f, 0.001f, 0.1f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        renderer->AddMesh(meshes[i]);
    }
}

void GraphView::SetRenderer(White::Engine::Graphics::Renderer* renderer) {
    this->renderer = renderer;
}
void GraphView::SetGraph(std::shared_ptr<Math::Graph> graph) {
    this->graph = graph;
}

void GraphView::SetGrid(std::shared_ptr<White::Grid> grid) {
    this->grid = grid;
}