#include "GraphView.h"
#include "Disk.h"
#include "Ring.h"
#include "Segment.h"
#include "DigitMeshCreator.h"
#include "CharacterBlock.h"
#include "ObjectManager.h"
#include "InterfaceProvider.h"
#include "ITransformable.h"
#include "ModelFormat.h"
#include "Model.h"

#include <algorithm>
#include <sstream>
#include <random>
#include <iterator>

using namespace White::Engine::Graphics;
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
    for (int i = 0; i < graph->GetVerticesCnt(); i++) {
        DisplayPost(i);
    }
    for (int i = 0; i < graph->GetEdgesCnt(); i++) {
        Edge* edge = graph->GetEdgeById(i);
        DisplayEdge(i);
    }
}

void GraphView::DisplayNode(int node) {
    ModelFormat format;
    format.numAttributes = 2;
    format.numShaders = 2;
    format.isTextured = false;
    format.isIndexed = true;
    format.numComponents.push_back(4);
    format.numComponents.push_back(4);
    format.shaders.push_back(L"Engine/Shaders/default.vsh");
    format.shaders.push_back(L"Engine/Shaders/default.fsh");
    Model model;
    model.SetFormat(format);

    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip;

    Math::Vector<float> pos = cells[shuffledIndices[node]].vertexPosition;

    Math::Vector<float> color = {1.0f, 1.0f, 0.0f, 1.0f};
    Math::Disk<float> disk(0.2);
    Mesh<float> diskMesh = disk.ToMesh(color, 90);
    unsigned mesh = om.Create<Mesh<float>>(diskMesh);
    auto ptr = ip.Query<ITransformable>(mesh);
    ip.Query<IScalable>(mesh)->Scale<float>({grid->cellSize[0], grid->cellSize[1], 1.0f});
    ip.Query<ITranslatable>(mesh)->Translate<float>({pos[0], pos[1], 0.7f});
    model.SetMesh(mesh);
    unsigned modelId = om.Create<Model>(model);
    renderer->AddModel(modelId);

    color = {0.0f, 0.0f, 0.5f, 1.0f};
    Math::Ring<float> ring(0.1, 0.28);
    Mesh<float> ringMesh = ring.ToMesh(color, 90); 
    mesh = om.Create<Mesh<float>>(ringMesh);
    ip.Query<IScalable>(mesh)->Scale<float>({grid->cellSize[0], grid->cellSize[1], 1.0f});
    ip.Query<ITranslatable>(mesh)->Translate<float>({pos[0], pos[1], 0.7f + 0.01f});
    model.SetMesh(mesh);
    modelId = om.Create<Model>(model);
    renderer->AddModel(modelId);
    
    std::stringstream str;
    str << graph->GetVById(node)->GetIdx();
    White::Engine::Graphics::CharacterBlock charBlock({pos[0], pos[1], 0.7f - 0.1f}, 
                                                      {grid->cellSize[1] / 3.0f, 
                                                       grid->cellSize[1] / 3.0f}, str.str());
    charBlock.Scale({grid->cellSize[0] / 6.0f, grid->cellSize[1] / 6.0f, 1.0f});
    charBlock.Translate({-0.008f, 0.0f, 0.0f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        mesh = om.Create<Mesh<float>>(meshes[i]);
        model.SetMesh(mesh);
        modelId = om.Create<Model>(model);
        renderer->AddModel(modelId);
    }
}

void GraphView::DisplayPost(int node) {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip;

    ModelFormat format;
    format.numAttributes = 2;
    format.numShaders = 2;
    format.isTextured = false;
    format.isIndexed = true;
    format.numComponents.push_back(4);
    format.numComponents.push_back(4);
    format.shaders.push_back(L"Engine/Shaders/default.vsh");
    format.shaders.push_back(L"Engine/Shaders/default.fsh");
    Model model;
    model.SetFormat(format);



    Math::Vector<float> pos = cells[shuffledIndices[node]].vertexPosition;

    Post* post = graph->GetVById(node)->GetPost();

    if (!post) {
        DisplayNode(node);
        return;
    }

    int type = post->GetPostType();
    MeshLoader loader;
    loader.format = format;
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
    ip.Query<IScalable>(postMesh)->Scale<float>({grid->cellSize[0], grid->cellSize[1], 1.0f});
    ip.Query<ITranslatable>(postMesh)->Translate<float>({pos[0], pos[1], 0.7f}); 
    model.SetMesh(postMesh);
    unsigned modelId = om.Create<Model>(model);
    renderer->AddModel(modelId); 
    renderer->AddMesh(postMesh);
}

void GraphView::DisplayEdge(int edge) {
    ObjectManager& om = ObjectManager::GetInstance();
    InterfaceProvider ip;
    
    ModelFormat format;
    format.numAttributes = 2;
    format.numShaders = 2;
    format.isTextured = false;
    format.isIndexed = true;
    format.numComponents.push_back(4);
    format.numComponents.push_back(4);
    format.shaders.push_back(L"Engine/Shaders/default.vsh");
    format.shaders.push_back(L"Engine/Shaders/default.fsh");
    Model model;
    model.SetFormat(format);

    Edge* edgePtr = graph->GetEdgeById(edge);
    int from = graph->GetVByIdx(edgePtr->GetFrom())->GetId();
    int to = graph->GetVByIdx(edgePtr->GetTo())->GetId();
    Math::Vector<float> color = {0.5f, 0.5f, 0.5f, 1.0f};
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
    ip.Query<IScalable>(seg)->Scale<float>({grid->cellSize[0], grid->cellSize[1] * 8, 1.0f});
    ip.Query<ITranslatable>(seg)->Translate<float>({mid[0], mid[1], 0.7f + 0.1}); 

    model.SetMesh(seg);
    unsigned modelId = om.Create<Model>(model);
    renderer->AddModel(modelId); 

    std::stringstream str;
    str << edgePtr->GetLength();
    White::Engine::Graphics::CharacterBlock charBlock({mid[0], mid[1], 0.7f - 0.01f}, 
                                                      {grid->cellSize[1] / 3.0f, grid->cellSize[1] / 3.0f}, str.str());
    charBlock.Scale({grid->cellSize[0] / 6.0f, grid->cellSize[1] / 6.0f, 1.0f});
    charBlock.Translate({-0.008f, 0.001f, 0.0f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        unsigned mesh = om.Create<Mesh<float>>(meshes[i]);
        model.SetMesh(mesh);
        modelId = om.Create<Model>(model);
        renderer->AddModel(modelId); 
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
