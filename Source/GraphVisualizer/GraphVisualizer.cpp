#include "GraphVisualizer.h"
#include "StartupSettings.h"
#include "InterfaceProvider.h"
#include "MeshLoader.h"
#include "ObjectManager.h"
#include "Logger.h"
#include "Renderer.h"
#include "Rectangle.h"
#include "Ring.h"
#include "ModelLoader.h"
#include "ITranslatable.h"
#include "Disk.h"
#include "Segment.h"
#include "Common.h"
#include "Grid.h"
#include "Connection.h"
#include "Camera.h"
#include "BMPLoader.h"

#include <time.h>

#include <atlbase.h>
#include <atlconv.h>

#include <sstream>
#include <fstream>
#include <iostream>

using namespace White::Util;
using namespace White::Engine::Util;
using namespace White::Engine::Graphics;

namespace White {
namespace Engine {
namespace Graphics {

GraphVisualizer::GraphVisualizer(Renderer& renderer) : Game(renderer) {
    prevTime = std::chrono::duration_cast<std::chrono::milliseconds>
               (std::chrono::system_clock::now().time_since_epoch());
    rotation = {0.0f, 0.0f, 0.0f};
}

DWORD GraphVisualizer::Listener() {
  Logger &l = Logger::GetInstance();
  while (true) {
        std::chrono::milliseconds curTime 
            = std::chrono::duration_cast<std::chrono::milliseconds>
              (std::chrono::system_clock::now().time_since_epoch());
        std::chrono::milliseconds time = curTime - prevTime;
       // clock_t start_time = clock();
        if (!overseer->Turn())
          break;
        //clock_t end_time = clock();
        //l << std::to_string((double)(end_time - start_time) / CLOCKS_PER_SEC) << std::string("\n");
        if (time > std::chrono::milliseconds(1)) {
            prevTime = curTime;
            graphView.UpdateTrains();
        }
    }
  overseer->Logout();
  return 0;
}

DWORD GraphVisualizer::ViewInitializer() {
    graph = overseer->GetGraph();
    int verticesCnt = graph->GetVerticesCnt();
    int dimension = 160 * (std::sqrt(verticesCnt) + 1); 
    grid.reset(new Grid({0.0f, dimension * 1.0f / 2.0f}, 
                        {dimension, dimension}, 
                        {1.0f, 1.0f}));
    graphView.SetRenderer(&renderer);
    graphView.SetGraph(graph);
    graphView.SetGrid(grid);
    graphView.Init();
    graphView.Display();
    camera.Rotate({0.0f, 180.0f, 0.0f});
    camera.Translate({0.0f, 40.0f, -6.0f});
    ObjectManager& om = ObjectManager::GetInstance();
    dir = {0.0f, 0.0f, 1.0f};
    prev = {0.0f, 0.0f};
    std::wifstream in(L"Engine/Config/startup.config");
    int n;
    in >> n;
    for (int i = 0; i < n; i++) {
        std::wstring modelPath;
        in >> modelPath;
        ModelLoader modelLoader;
        Model cube = modelLoader.Import(modelPath);
        unsigned cubeModel = om.Create<Model>(cube);
        renderer.AddModel(cubeModel);
    }
    renderer.UpdateVertexData();
    renderer.ToggleActualState();
    graphView.CompleteInitialization();

    return 0;
}



void GraphVisualizer::LoadGraph(std::string name) {
    graph = std::make_shared<Graph>();
    Connection& conn = Connection::GetInstance(SERVER_ADDR, SERVER_PORT);
    std::string data = "{\"name\":\"" + name + "\"}";
    ActionMessage msg = conn.FormActionMessage(Action::LOGIN, data);
    ResponseMessage resp;
    conn.Request(msg, resp);
    delete[](resp.data);
    delete[](msg.data);
    msg = conn.FormActionMessage(Action::MAP, conn.LAYER0);
    conn.Request(msg, resp);
    if (resp.result == Result::OKEY) {
        ParseGraphFromJSON(graph, resp.data);
    }
    delete[](resp.data);
    delete[](msg.data);
    msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
    conn.Request(msg, resp);
    if (resp.result == Result::OKEY) {
        ParseInfrastructureFromJSON(graph, resp.data);
    }
    delete[](resp.data);
    delete[](msg.data);
}

void GraphVisualizer::UpdateCamera() {
    RECT rect;
    HWND hWnd = StartupSettings::GetInstance().GetWindowHandle(); 
    GetWindowRect(hWnd, &rect);
    float w = (rect.right - rect.left) / 2.0f;
    float h = (rect.bottom - rect.top) / 2.0f;
    float width = 0.1;
    float height = h * width / w;
    Matrix<GLfloat> view = camera.GetViewMatrix();
    Matrix<GLfloat> projection = Matrix<GLfloat>::Projection(-width, height, 
                                                             width, -height, 
                                                             0.1f, 10000000.0f);
    renderer.SetView(view);
    renderer.SetProjection(projection);
}

void GraphVisualizer::Play() {
    if (!overseer) {
        //logger.Log(2);
        StartupSettings& settings = StartupSettings::GetInstance();
        settings.ParseCommandLineArgs();
        std::vector<std::wstring> args = settings.GetArgs();
        //std::wstring graphPath = args[1];
        std::wstring playerName = args[1];
        std::wstring gameID = args[1];
        std::wstring num_players = args[1];
        std::wstring num_turns = args[1];
        if (args.size() > 2)
          gameID = args[2];
        if (args.size() > 3)
          num_players = args[3];
        if (args.size() > 4)
          num_turns = args[4];
        //CW2A cw2a(graphPath.c_str());
        CW2A cw2a(playerName.c_str());
        CW2A cw2a_1(gameID.c_str());
        CW2A cw2a_2(num_players.c_str());
        CW2A cw2a_3(num_turns.c_str());
        // std::string path = cw2a;
        std::string name = cw2a;
        std::string game = cw2a_1;
        std::string players = cw2a_2;
        std::string turns = cw2a_3;
        //LoadGraph(path); 
        overseer = std::make_shared<Overseer>();
        Logger& l = Logger::GetInstance();
        l << args.size();
        //logger << 2.1;
        overseer->Init(name, game, players, turns);

        ///////
    //graph = overseer->GetGraph();
    //int verticesCnt = graph->GetVerticesCnt();
    //int dimension = 160 * (std::sqrt(verticesCnt) + 1); 
    //grid.reset(new Grid({0.0f, dimension * 1.0f / 2.0f}, 
    //                    {dimension, dimension}, 
    //                    {1.0f, 1.0f}));
    //graphView.SetRenderer(&renderer);
    //graphView.SetGraph(graph);
    //graphView.SetGrid(grid);
    //graphView.Init();
    //graphView.Display();
    //camera.Rotate({0.0f, 180.0f, 0.0f});
    //camera.Translate({0.0f, 40.0f, -6.0f});
    //ObjectManager& om = ObjectManager::GetInstance();
    //dir = {0.0f, 0.0f, 1.0f};
    //prev = {0.0f, 0.0f};
    //std::wifstream in(L"Engine/Config/startup.config");
    //int n;
    //in >> n;
    //for (int i = 0; i < n; i++) {
    //    std::wstring modelPath;
    //    in >> modelPath;
    //    ModelLoader modelLoader;
    //    Model cube = modelLoader.Import(modelPath);
    //    unsigned cubeModel = om.Create<Model>(cube);
    //    renderer.AddModel(cubeModel);
    //}
    //renderer.UpdateVertexData();
    //graphView.CompleteInitialization();
    graph = overseer->GetGraph();
    int verticesCnt = graph->GetVerticesCnt();
    int dimension = 160 * (std::sqrt(verticesCnt) + 1); 
    grid.reset(new Grid({0.0f, dimension * 1.0f / 2.0f}, 
                        {dimension, dimension}, 
                        {1.0f, 1.0f}));
    graphView.SetRenderer(&renderer);
    graphView.SetGraph(graph);
    graphView.SetGrid(grid);
    graphView.Init();

        overseerThread.Start(this, &GraphVisualizer::Listener);

    graphView.Display();
    camera.Rotate({0.0f, 180.0f, 0.0f});
    camera.Translate({0.0f, 40.0f, -6.0f});
    ObjectManager& om = ObjectManager::GetInstance();
    dir = {0.0f, 0.0f, 1.0f};
    prev = {0.0f, 0.0f};
    std::wifstream in(L"Engine/Config/startup.config");
    int n;
    in >> n;
    for (int i = 0; i < n; i++) {
        std::wstring modelPath;
        in >> modelPath;
        ModelLoader modelLoader;
        Model cube = modelLoader.Import(modelPath);
        unsigned cubeModel = om.Create<Model>(cube);
        renderer.AddModel(cubeModel);
    }
    renderer.UpdateVertexData();
    renderer.ToggleActualState();
    graphView.CompleteInitialization();






        /////////////
    //    initializerThread.Start(this, &GraphVisualizer::ViewInitializer);
    }
    InterfaceProvider ip;
    UpdateCamera();
    POINT point;
    GetCursorPos(&point);
    Vector<float> cur = {static_cast<float>(point.x), 
                         static_cast<float>(point.y)};        
    int lButtonKeyState = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    if (lButtonKeyState) {
        rotation += {-(cur[1] - prev[1]) / 3.7f, 
                     (cur[0] - prev[0]) / 3.7f, 0.0f};
        for (int i = 0; i < 3; i++) {
            if (rotation[i] > 360.0f) {
                rotation[i] = rotation[i] - 360.0f;
            }
        }
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationMat *  dir;
        camera.Rotate({-(cur[1] - prev[1]) / 3.7f, 
                       (cur[0] - prev[0]) / 3.7f, 0.0f});
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        keys[0] = -keys[0];
        Matrix<float> rotationRight = Matrix<float>::Rotation({0.0f, 
                                                               90.0f, 0.0f});
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationRight * rotationMat *  dir; 
        camera.Translate({-dir[0] * 6.0f, +0.0f, -dir[2] * 6.0f});
    } 
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        keys[1] = -keys[1];
        Matrix<float> rotationRight = Matrix<float>::Rotation({0.0f, 
                                                               -90.0f, 0.0f});
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationRight * rotationMat *  dir; 

        camera.Translate({-dir[0] * 6.0f, +0.0f, -dir[2] * 6.0f});
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        keys[2] = -keys[2];
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationMat *  dir;  

        camera.Translate({dir[0] * 6.0f, +0.0f, dir[2] * 6.0f});

    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        keys[3] = -keys[3];
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationMat *  dir;  

        camera.Translate({-dir[0] * 6.0f, -0.0f, -dir[2] * 6.0f});
    }
    if (GetAsyncKeyState(0x41) & 0x8000) {
        keys[0] = -keys[0];
        Matrix<float> rotationRight = Matrix<float>::Rotation({0.0f, 
                                                               90.0f, 0.0f});
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationRight * rotationMat *  dir; 

        camera.Translate({-dir[0] * 6.0f, +0.0f, -dir[2] * 6.0f});

    } 
    if (GetAsyncKeyState(0x44) & 0x8000) {
        keys[1] = -keys[1];
        Matrix<float> rotationRight = Matrix<float>::Rotation({0.0f, 
                                                               -90.0f, 0.0f});
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationRight * rotationMat *  dir; 
        camera.Translate({-dir[0] * 6.0f, +0.0f, -dir[2] * 6.0f});

    }
    if (GetAsyncKeyState(0x57) & 0x8000) {
        keys[2] = -keys[2];
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationMat *  dir;  
        camera.Translate({dir[0] * 6.0f, +0.0f, dir[2] * 6.0f});
    }
    if (GetAsyncKeyState(0x53) & 0x8000) {
        keys[3] = -keys[3];
        Matrix<float> rotationMat 
            = Matrix<float>::Rotation(rotation);
        dir = {0.0f, 0.0f, 1.0f};
        dir = rotationMat *  dir;  
        camera.Translate({-dir[0] * 6.0f, -0.0f, -dir[2] * 6.0f});
    } 
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        keys[9] = -keys[9];
        camera.Translate({0.0f, 5.0f, 0.0f});
    }
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        keys[10] = -keys[10];
        camera.Translate({0.0f, -5.0f, 0.0f});
    } 
    prev = cur;
}

}
}
}
