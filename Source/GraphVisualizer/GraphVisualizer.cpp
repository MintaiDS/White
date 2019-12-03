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

GraphVisualizer::GraphVisualizer(Renderer& renderer) 
        : Game(renderer) {

    //Logger& l = Logger::GetInstance();
    //l.Init("run.log");

    prevTime = std::chrono::duration_cast<std::chrono::milliseconds>
           (std::chrono::system_clock::now().time_since_epoch());
}

DWORD GraphVisualizer::Listener() {
    while (true) {
        std::chrono::milliseconds curTime 
            = std::chrono::duration_cast<std::chrono::milliseconds>
              (std::chrono::system_clock::now().time_since_epoch());
        std::chrono::milliseconds time = curTime - prevTime;
        if (time > std::chrono::milliseconds(1550)) {
            prevTime = curTime;
            overseer->Turn();
            graphView.UpdateTrains();
        }
    }

    return 0;
}

void GraphVisualizer::LoadGraph(std::string name) {
    // graph.reset(ParseGraphFromJSON(path));
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
    if (resp.result == Result::OKEY)
    {

      ParseGraphFromJSON(graph, resp.data);

    }
    delete[](resp.data);
    delete[](msg.data);
    msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
    conn.Request(msg, resp);
    if (resp.result == Result::OKEY)
    {

      ParseInfrastructureFromJSON(graph, resp.data);

    }
    delete[](resp.data);
    delete[](msg.data);
    //graph.reset(&gr);
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
        //CW2A cw2a(graphPath.c_str());
        CW2A cw2a(playerName.c_str());
        // std::string path = cw2a;
        std::string name = cw2a;
        //LoadGraph(path); 
        overseer = std::make_shared<Overseer>();
        //logger << 2.1;
        overseer->Init(name);
        //logger << 2.2;
        graph = overseer->GetGraph();
        //logger.Log(3);
        //LoadGraph(name); 
        //logger.Log(4);
        //logger.Log(graph->GetVerticesCnt());
        int verticesCnt = graph->GetVerticesCnt();

        //logger.Log(5);

        int dimension = 50 * (std::sqrt(verticesCnt) + 1); 
        grid.reset(new Grid({0.0f, dimension * 1.0f / 2.0f}, 
                            {dimension, dimension}, 
                            {1.0f, 1.0f}));

        //logger.Log(6);

        graphView.SetRenderer(&renderer);
        graphView.SetGraph(graph);
        graphView.SetGrid(grid);
        graphView.Init();
        //logger.Log(7);
        graphView.Display();
        camera.Rotate({0.0f, 180.0f, 0.0f});
        camera.Translate({0.0f, 1.0f, -6.0f});
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
        overseerThread.Start(this, &GraphVisualizer::Listener);
    }
    InterfaceProvider ip;
    UpdateCamera();
    POINT point;
    GetCursorPos(&point);
    Vector<float> cur = {static_cast<float>(point.x), 
                         static_cast<float>(point.y)};        
    int lButtonKeyState = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    if (lButtonKeyState) {
        Matrix<float> rotation 
            = Matrix<float>::Rotation({-(cur[1] - prev[1]) / 3.7f, 
                                       (cur[0] - prev[0]) / 3.7f, 0.0f});
        dir = rotation * dir;
        camera.Rotate({-(cur[1] - prev[1]) / 3.7f, 
                       (cur[0] - prev[0]) / 3.7f, 0.0f});
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        keys[0] = -keys[0];
        Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                          90.0f, 0.0f});
        dir = rotation * dir;
        camera.Translate({-dir[0] / 3.0f, +0.0f, -dir[2] / 3.0f});
        rotation = Matrix<float>::Rotation({0.0f, -90.0f, 0.0f});  
        dir = rotation * dir;
    } 
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        keys[1] = -keys[1];
        Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                          -90.0f, 0.0f});
        dir = rotation * dir;
        camera.Translate({-dir[0] / 3.0f, +0.0f, -dir[2] / 3.0f});
        rotation = Matrix<float>::Rotation({0.0f, 90.0f, 0.0f});  
        dir = rotation * dir;
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        keys[2] = -keys[2];
        camera.Translate({dir[0] / 3.0f, +0.0f, dir[2] / 3.0f});
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        keys[3] = -keys[3];
        camera.Translate({-dir[0] / 3.0f, -0.0f, -dir[2] / 3.0f});
    }
    if (GetAsyncKeyState(0x41) & 0x8000) {
        keys[0] = -keys[0];
        Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                          90.0f, 0.0f});
        dir = rotation * dir;
        camera.Translate({-dir[0] / 3.0f, +0.0f, -dir[2] / 3.0f});
        rotation = Matrix<float>::Rotation({0.0f, -90.0f, 0.0f});  
        dir = rotation * dir;
    } 
    if (GetAsyncKeyState(0x44) & 0x8000) {
        keys[1] = -keys[1];
        Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                          -90.0f, 0.0f});
        dir = rotation * dir;
        camera.Translate({-dir[0] / 3.0f, +0.0f, -dir[2] / 3.0f});
        rotation = Matrix<float>::Rotation({0.0f, 90.0f, 0.0f});  
        dir = rotation * dir;
    }
    if (GetAsyncKeyState(0x57) & 0x8000) {
        keys[2] = -keys[2];
        camera.Translate({dir[0] / 3.0f, +0.0f, dir[2] / 3.0f});
    }
    if (GetAsyncKeyState(0x53) & 0x8000) {
        keys[3] = -keys[3];
        camera.Translate({-dir[0] / 3.0f, -0.0f, -dir[2] / 3.0f});
    } 
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        keys[9] = -keys[9];
        camera.Translate({0.0f, 0.5f, 0.0f});
    }
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        keys[10] = -keys[10];
        camera.Translate({-0.0f, -0.5f, -0.0f});
    } 
    prev = cur;
    //overseer->Turn();
    //graphView.UpdateTrains();
}

}
}
}
