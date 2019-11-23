#include "GraphView.h"
#include "Disk.h"
#include "Ring.h"
#include "Segment.h"
#include "DigitMeshCreator.h"
#include "CharacterBlock.h"
#include "ObjectManager.h"
#include "InterfaceProvider.h"
#include "ITransformable.h"

#include <algorithm>
#include <sstream>
#include <random>
#include <iterator>

using namespace White::Util;
using namespace White::Engine;
using namespace White;

GraphView::GraphView() 
        : renderer(nullptr) {}

void GraphView::Init() {
    int cnt = grid->gridSize[1] * grid->gridSize[1];
    for (int i = 0; i < cnt; i++) {
        int row = i / (grid->gridSize[1]);
        int column = i % (grid->gridSize[1]);
        cells.push_back(grid->cells[row][column]);
        shuffledIndices.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd()); 
    std::shuffle(shuffledIndices.begin(), shuffledIndices.end(), g);
}

void GraphView::Display() {
    //ObjectManager& om = ObjectManager::GetInstance();
    //InterfaceProvider ip;
    //MeshLoader loader;
    //Mesh<float> mesh;
    //loader.Import(L"Engine/Models/Shapes/cube.polygon");
    //mesh = loader.mesh;
    //unsigned newMesh = om.Create<Mesh<float>>(mesh);
    ////ip.Query<IScalable>(newMesh)->Scale<float>({0.12f, 0.12f, 1.0f});
    //ip.Query<ITranslatable>(newMesh)->Translate<float>({0.0f, 0.0f, 2.0f}); 
    //renderer->AddMesh(newMesh);

    ////for (int i = 0; i < graph->GetVerticesCnt(); i++) {
    ////    DisplayPost(i);
    ////}
    ////for (int i = 0; i < graph->GetEdgesCnt(); i++) {
    ////    Edge* edge = graph->GetEdgeById(i);
    ////    DisplayEdge(i);
    ////}
    //renderer->UpdateVertexData();
}

void GraphView::DisplayNode(int node) {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip;

    Math::Vector<float> pos = cells[shuffledIndices[node]].vertexPosition;

    Math::Vector<float> color = {1.0f, 1.0f, 0.0f, 1.0f};
    Math::Disk<float> disk(0.2);
    Mesh<float> diskMesh = disk.ToMesh(color, 120);
    unsigned mesh = om.Create<Mesh<float>>(diskMesh);
    auto ptr = ip.Query<ITransformable>(mesh);
    ip.Query<IScalable>(mesh)->Scale<float>({0.2f, 0.2f, 1.0f});
    ip.Query<ITranslatable>(mesh)->Translate<float>({pos[0], pos[1], 0.7f});
    renderer->AddMesh(mesh);

    color = {0.0f, 0.0f, 0.5f, 1.0f};
    Math::Ring<float> ring(0.1, 0.28);
    Mesh<float> ringMesh = ring.ToMesh(color, 120); 
    mesh = om.Create<Mesh<float>>(ringMesh);
    ip.Query<IScalable>(mesh)->Scale<float>({0.2f, 0.2f, 1.0f});
    ip.Query<ITranslatable>(mesh)->Translate<float>({pos[0], pos[1], 0.7f});
    renderer->AddMesh(mesh);
    
    std::stringstream str;
    str << graph->GetVById(node)->GetIdx();
    White::Engine::Graphics::CharacterBlock charBlock({pos[0], pos[1], 0.7f - 0.00001f}, 
                                                      {0.032f, 0.032f}, str.str());
    charBlock.Scale({0.028f, 0.028f, 1.0f});
    charBlock.Translate({-0.008f, 0.0f, 0.0f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        mesh = om.Create<Mesh<float>>(meshes[i]);
        renderer->AddMesh(mesh);
    }
}

void GraphView::DisplayPost(int node) {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip;

    Math::Vector<float> pos = cells[shuffledIndices[node]].vertexPosition;

    Post* post = graph->GetVById(node)->GetPost();

    if (!post) {
        DisplayNode(node);
        return;
    }

    int type = post->GetPostType();
    MeshLoader loader;
    Mesh<float> mesh;
    switch (type) {
    case CITY:
        loader.Import(L"Engine/Models/Map/city.polygon");
        break;
    case MARKET:
        loader.Import(L"Engine/Models/Map/market.polygon");
        break;
    case STORAGE: 
        loader.Import(L"Engine/Models/Map/storage.polygon");
        break;
    }
    mesh = loader.mesh;

    unsigned postMesh = om.Create<Mesh<float>>(mesh);
    ip.Query<IScalable>(postMesh)->Scale<float>({0.12f, 0.12f, 1.0f});
    ip.Query<ITranslatable>(postMesh)->Translate<float>({pos[0], pos[1], 0.7f}); 
    renderer->AddMesh(postMesh);
}

void GraphView::DisplayEdge(int edge) {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip;

    Edge* edgePtr = graph->GetEdgeById(edge);
    int from = graph->GetVByIdx(edgePtr->GetFrom())->GetId();
    int to = graph->GetVByIdx(edgePtr->GetTo())->GetId();
    Math::Vector<float> color = {0.7f, 0.7f, 0.6f, 1.0f};
    Math::Vector<float> begin = cells[shuffledIndices[from]].vertexPosition;
    Math::Vector<float> end = cells[shuffledIndices[to]].vertexPosition; 
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
    unsigned seg = om.Create<Mesh<float>>(segmentMesh);
    ip.Query<IRotatable>(seg)->Rotate<float>(rotation);
    ip.Query<ITranslatable>(seg)->Translate<float>({mid[0], mid[1], 0.7f}); 
    renderer->AddMesh(seg);

    std::stringstream str;
    str << edgePtr->GetLength();
    White::Engine::Graphics::CharacterBlock charBlock({mid[0], mid[1], 0.7f - 0.00001f}, 
                                                      {0.032f, 0.032f}, str.str());
    charBlock.Scale({0.028f, 0.028f, 1.0f});
    charBlock.Translate({-0.008f, 0.001f, 0.0f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        unsigned mesh = om.Create<Mesh<float>>(meshes[i]);
        renderer->AddMesh(mesh);
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
