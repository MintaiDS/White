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
#include "Train.h"
#include "Model.h"
#include "Logger.h"

#include <algorithm>
#include <sstream>
#include <random>
#include <iterator>

using namespace White::Engine::Graphics;
using namespace White::Util;
using namespace White::Engine;
using namespace White;

GraphView::GraphView() 
        : renderer(nullptr) {
    ObjectManager& om = ObjectManager::GetInstance();
    mainMesh = om.Create<Mesh<float>>();
    mainModel = om.Create<Model>();
}

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
    for (auto& p : graph->GetEdges()) {
        DisplayEdge(p.second);
    }
    for (auto& p : graph->GetTrains()) {
        DisplayTrain(p.second);
    }
    ModelFormat format;
    format.numAttributes = 0;
    format.numShaders = 0;
    format.isTextured = false;
    format.isIndexed = false;
   InterfaceProvider ip;
    auto model = ip.Query<Model>(mainModel);
    model->SetFormat(format);
    model->SetMesh(mainMesh);
    auto mesh = ip.Query<Mesh<float>>(mainMesh);
    mesh->Rotate({90.0f, 0.0f, 0.0f});
    mesh->Translate({0.0f, 6.0f, 0.0f});
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
    ip.Query<IScalable>(mesh)->Scale<float>({grid->cellSize[0] * 18, 
                                             grid->cellSize[1] * 18, 1.0f});
    ip.Query<ITranslatable>(mesh)->Translate<float>({pos[0], pos[1], 0.7f});
    model.SetMesh(mesh);
    unsigned modelId = om.Create<Model>(model);
    renderer->AddModel(modelId);
    ip.Query<Mesh<float>>(mainMesh)->AddChild(mesh);
    color = {0.0f, 0.0f, 0.5f, 1.0f};
    Math::Ring<float> ring(0.1, 0.28);
    Mesh<float> ringMesh = ring.ToMesh(color, 90); 
    mesh = om.Create<Mesh<float>>(ringMesh);
    ip.Query<IScalable>(mesh)->Scale<float>({grid->cellSize[0] * 18, 
                                             grid->cellSize[1] * 18, 1.0f});
    ip.Query<ITranslatable>(mesh)->Translate<float>({pos[0], 
                                                     pos[1], 0.7f + 0.01f});
    model.SetMesh(mesh);
    modelId = om.Create<Model>(model);
    renderer->AddModel(modelId);
    ip.Query<Mesh<float>>(mainMesh)->AddChild(mesh);
    std::stringstream str;
    str << graph->GetVById(node)->GetIdx();
    White::Engine::Graphics::CharacterBlock charBlock(
                                            {pos[0], pos[1], 0.7f - 0.2f}, 
                                            {grid->cellSize[1] * 4.4f, 
                                             grid->cellSize[1] * 4.4f}, 
                                             str.str());
    charBlock.Scale({grid->cellSize[0] * 4.4f, 
                     grid->cellSize[1] * 4.4f, 1.0f});
    charBlock.Translate({-0.008f, 0.0f, 0.0f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        mesh = om.Create<Mesh<float>>(meshes[i]);
        model.SetMesh(mesh);
        modelId = om.Create<Model>(model);
        renderer->AddModel(modelId);
        ip.Query<Mesh<float>>(mainMesh)->AddChild(mesh);
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
    ip.Query<IScalable>(postMesh)->Scale<float>({grid->cellSize[0] * 7, grid->cellSize[1] * 7, 3.0f});
    ip.Query<ITranslatable>(postMesh)->Translate<float>({pos[0], pos[1], 0.7f}); 
    ip.Query<IRotatable>(postMesh)->Rotate<float>({-90.0f, 0.0f, 0.0f});
    model.SetMesh(postMesh);
    unsigned modelId = om.Create<Model>(model);
    renderer->AddModel(modelId); 
    ip.Query<Mesh<float>>(mainMesh)->AddChild(postMesh);
    renderer->AddMesh(postMesh);
}


void GraphView::DisplayEdge(Edge* edgePtr) {
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
    ip.Query<IScalable>(seg)->Scale<float>({grid->cellSize[0], grid->cellSize[1] * 135, 1.0f});
    ip.Query<ITranslatable>(seg)->Translate<float>({mid[0], mid[1], 0.7f + 0.1}); 

    model.SetMesh(seg);
    unsigned modelId = om.Create<Model>(model);
    ip.Query<Mesh<float>>(mainMesh)->AddChild(seg);
    renderer->AddModel(modelId); 

    std::stringstream str;
    str << edgePtr->GetLength();
    White::Engine::Graphics::CharacterBlock charBlock({mid[0], mid[1], 0.7f - 0.2f}, 
                                                      {grid->cellSize[1] * 4.4f, 
                                                      grid->cellSize[1] * 4.4f}, str.str());
    charBlock.Scale({grid->cellSize[0] * 4.4f, grid->cellSize[1] * 4.4f, 1.0f});
    charBlock.Translate({-0.008f, 0.001f, 0.0f});
    std::vector<Mesh<float>>& meshes = charBlock.GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        unsigned mesh = om.Create<Mesh<float>>(meshes[i]);
        model.SetMesh(mesh);
        modelId = om.Create<Model>(model);
        renderer->AddModel(modelId); 
        ip.Query<Mesh<float>>(mainMesh)->AddChild(mesh);
        renderer->AddMesh(mesh);
    }
}


void GraphView::DisplayTrain(Train* trainObj) {
    Edge* edgePtr = graph->GetEdgeByIdx(trainObj->GetLineIdx());
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
    float len = dir.Length();
    float step = len / edgePtr->GetLength();
    dir *= (1.0f / len);
    dir *= step;
    Math::Vector<float> position = begin + dir * trainObj->GetPosition();
    MeshLoader loader;
    loader.format = format;
    Mesh<float> mesh;
    loader.Import(L"Engine/Models/Map/train.polygon");
    mesh = loader.mesh;
    unsigned trainMesh = om.Create<Mesh<float>>(mesh); 
    trains.push_back(trainMesh);
    model.SetMesh(trainMesh);
    unsigned modelId = om.Create<Model>(model);
    renderer->AddModel(modelId); 
    ip.Query<Mesh<float>>(mainMesh)->AddChild(trainMesh);
    renderer->AddMesh(trainMesh);
    ip.Query<IRotatable>(trainMesh)->Rotate<float>(rotation);
    ip.Query<IScalable>(trainMesh)->Scale<float>({6.0f, 4.0f, 1.0f});
    ip.Query<ITranslatable>(trainMesh)->Translate<float>({position[0], position[1], 0.7f + 0.1}); 
}

void GraphView::UpdateTrains() {
    InterfaceProvider ip;

    int i = 0;
    for (auto& p : graph->GetTrains()) {
        Train* trainObj = p.second;

        auto mesh = ip.Query<Mesh<float>>(trains[i]);
        Math::Vector<float> translation = mesh->GetTranslation();
        mesh->Translate(translation * -1);
        Edge* edgePtr = graph->GetEdgeByIdx(trainObj->GetLineIdx());
        int from = graph->GetVByIdx(edgePtr->GetFrom())->GetId();
        int to = graph->GetVByIdx(edgePtr->GetTo())->GetId();
        Math::Vector<float> begin = cells[shuffledIndices[from]].vertexPosition;
        Math::Vector<float> end = cells[shuffledIndices[to]].vertexPosition; 
        Math::Vector<float> dir = end - begin; 
        float len = dir.Length();
        float step = len / edgePtr->GetLength();
        dir *= (1.0f / len);
        dir *= step;
        Math::Vector<float> position = begin + dir * trainObj->GetPosition();
        mesh->Translate({position[0], position[1], 0.7f + 0.1});
        i++;

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
