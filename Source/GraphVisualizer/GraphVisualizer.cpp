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

#include <AtlBase.h>
#include <atlconv.h>

#include <sstream>
#include <fstream>
#include <iostream>

using namespace White::Engine::Util;
using namespace White::Engine::Graphics;

namespace White {
namespace Engine {
namespace Graphics {

GraphVisualizer::GraphVisualizer(Renderer& renderer) 
        : Game(renderer) {}

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
      ParseGraphFromJSON(*graph, resp.data);
    }
    delete[](resp.data);
    delete[](msg.data);
    msg = conn.FormActionMessage(Action::MAP, conn.LAYER1);
    conn.Request(msg, resp);
    if (resp.result == Result::OKEY)
    {
      ParseInfrastructureFromJSON(*graph, resp.data);
    }
    delete[](resp.data);
    delete[](msg.data);
    //graph.reset(&gr);
}

void GraphVisualizer::UpdateCamera() {
    
    Matrix<GLfloat> view = camera.GetViewMatrix();
    //std::unique_ptr<GLfloat[]> raw 
    //    = std::make_unique<GLfloat[]>(view.rows * view.columns);
    //program.Use();
    //GLint location = glGetUniformLocation(program.id, "view");
    //for (int i = 0; i < view.rows; i++) {
    //    for (int j = 0; j < view.columns; j++) {
    //        raw.get()[i * view.columns + j] = view[i][j];
    //    }
    //}
    //glUniformMatrix4fv(location, 1, GL_TRUE, raw.get());

    //GLfloat scaleFactor = (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;

    //Matrix<GLfloat> projection = Matrix<GLfloat>::Identity(4);
    Matrix<GLfloat> projection = Matrix<GLfloat>::Projection(-0.1f, 0.1f, 
                                                             0.1f, -0.1f, 
                                                             0.1f, 1000.0f);
    //for (int i = 0; i < projection.rows; i++) {
    //    for (int j = 0; j < projection.columns; j++) {
    //        raw.get()[i * projection.columns + j] = projection[i][j];
    //    }
    //}
    //location = glGetUniformLocation(program.id, "projection");
    //glUniformMatrix4fv(location, 1, GL_TRUE, raw.get());

    renderer.SetView(view);
    renderer.SetProjection(projection);
}

void GraphVisualizer::Play() {
    if (!graph) {
        StartupSettings& settings = StartupSettings::GetInstance();
        settings.ParseCommandLineArgs();
        std::vector<std::wstring> args = settings.GetArgs();
        //std::wstring graphPath = args[1];
        std::wstring playerName = args[1];
        //CW2A cw2a(graphPath.c_str());
        CW2A cw2a(playerName.c_str());
        // std::string path = cw2a;
        std::string name = cw2a;
        // LoadGraph(path); 
        LoadGraph(name); 
        int verticesCnt = graph->GetVerticesCnt();
        int dimension = 2 * (std::sqrt(verticesCnt) + 1); 
        grid.reset(new Grid({0.0f, 0.0f}, 
                            {dimension, dimension}, 
                            {0.25f, 0.25f}));
        UpdateCamera();
        //graphView.SetRenderer(&renderer);
        //graphView.SetGraph(graph);
        //graphView.SetGrid(grid);
        //graphView.Init();
        //graphView.Display();
        //cameraScaling = 1.0f;
        //cameraScalingStep = 0.03f;
        //cameraTranslation = {0, 0, 0};
        ////camera.Translate({0.0f, 0.0f, -0.5f});
        //camera.Scale({1.0f, 1.0f, 1.0f});
        //GLfloat scaleFactor = (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
        camera.Rotate({0.0f, 180.0f, 0.0f});
        camera.Translate({0.0f, 0.0f, -1.0f});

        ObjectManager& om = ObjectManager::GetInstance();
        //InterfaceProvider ip;
        //MeshLoader loader;
        //Mesh<float> meshTerrain;
        //Mesh<float> meshSky;
        //Mesh<float> meshLine;
        //loader.Import(L"Engine/Models/Objects/wall_gray.polygon");
        //meshTerrain = loader.mesh;
        //loader.Import(L"Engine/Models/Objects/wall_blue.polygon");
        //meshSky = loader.mesh;
        //loader.Import(L"Engine/Models/Objects/line_gray.polygon");
        //meshLine = loader.mesh;
        //cubeId = om.Create<Mesh<float>>(meshSky);
        ////ip.Query<IScalable>(cubeId)->Scale<float>({0.12f, 0.12f, 1.0f});
        //ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, 0.0f, 100.0f}); 
        //renderer.AddMesh(cubeId);
        //
        //cubeId = om.Create<Mesh<float>>(meshSky);
        ////ip.Query<IScalable>(cubeId)->Scale<float>({0.12f, 0.12f, 1.0f});
        //ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, 0.0f, -100.0f}); 
        //renderer.AddMesh(cubeId);

        //cubeId = om.Create<Mesh<float>>(meshSky);
        ////ip.Query<IScalable>(cubeId)->Scale<float>({0.12f, 0.12f, 1.0f});
        //ip.Query<ITranslatable>(cubeId)->Translate<float>({-100.0f, 0.0f, 0.0f}); 
        //ip.Query<IRotatable>(cubeId)->Rotate<float>({0.0f, 90.0f, 0.0f}); 
        //renderer.AddMesh(cubeId);

        //cubeId = om.Create<Mesh<float>>(meshSky);
        ////ip.Query<IScalable>(cubeId)->Scale<float>({0.12f, 0.12f, 1.0f});
        //ip.Query<ITranslatable>(cubeId)->Translate<float>({100.0f, 0.0f, 0.0}); 
        //ip.Query<IRotatable>(cubeId)->Rotate<float>({0.0f, 90.0f, 0.0f}); 
        //renderer.AddMesh(cubeId);


        //cubeId = om.Create<Mesh<float>>(meshTerrain);
        ////ip.Query<IScalable>(cubeId)->Scale<float>({0.12f, 0.12f, 1.0f});
        //ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, -100.0f, 0.0f}); 
        //ip.Query<IRotatable>(cubeId)->Rotate<float>({90.0f, 0.0f, 0.0f}); 
        //renderer.AddMesh(cubeId);

        //cubeId = om.Create<Mesh<float>>(meshSky);
        ////ip.Query<IScalable>(cubeId)->Scale<float>({0.12f, 0.12f, 1.0f});
        //ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, 100.0f, 0.0f}); 
        //ip.Query<IRotatable>(cubeId)->Rotate<float>({90.0f, 0.0f, 0.0f}); 
        //renderer.AddMesh(cubeId);

        //unsigned lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({-100.0f, 0.0f, 100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({100.0f, 0.0f, 100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({100.0f, 0.0f, -100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({-100.0f, 0.0f, -100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({100.0f, 100.0f, 0.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({90.0f, 0.0f, 0.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({-100.0f, 100.0f, 0.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({90.0f, 0.0f, 0.0f}); 
        //renderer.AddMesh(lineId);


        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({100.0f, -100.0f, 0.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({90.0f, 0.0f, 0.0f}); 
        //renderer.AddMesh(lineId);


        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({-100.0f, -100.0f, 0.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({90.0f, 0.0f, 0.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({0.0f, 100.0f, 100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({0.0f, 0.0f, 90.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({0.0f, 100.0f, -100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({0.0f, 0.0f, 90.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({0.0f, -100.0f, 100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({0.0f, 0.0f, 90.0f}); 
        //renderer.AddMesh(lineId);

        //lineId = om.Create<Mesh<float>>(meshLine);
        //ip.Query<ITranslatable>(lineId)->Translate<float>({0.0f, -100.0f, -100.0f}); 
        //ip.Query<IScalable>(lineId)->Scale<float>({1.0, 100.0f, 1.0f}); 
        //ip.Query<IRotatable>(lineId)->Rotate<float>({0.0f, 0.0f, 90.0f}); 
        //renderer.AddMesh(lineId);
        dir = {0.0f, 0.0f, 1.0f};
        prev = {0.0f, 0.0f};

        ModelLoader modelLoader;
        Model cube = modelLoader.Import(L"Engine/Models/Shapes/cube.model");
        unsigned cubeModel = om.Create<Model>(cube);
        renderer.AddModel(cubeModel);
        renderer.UpdateVertexData();

        UpdateCamera();
    }
    InterfaceProvider ip;
    if (mode == 1) {
        //if ((GetAsyncKeyState(VK_LEFT) < 0) != keys[0]) {
        //    keys[0] = -keys[0];
        //    ip.Query<ITranslatable>(cubeId)->Translate<float>({-0.01f, 0.0f, 0.0f});
        //} 
        //if ((GetAsyncKeyState(VK_RIGHT) < 0) != keys[1]) {
        //    keys[1] = -keys[1];
        //    ip.Query<ITranslatable>(cubeId)->Translate<float>({0.01f, 0.0f, 0.0f});
        //}
        //if ((GetAsyncKeyState(VK_UP) < 0) != keys[2]) {
        //    keys[2] = -keys[2];
        //    ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, +0.01f, 0.0f});
        //}
        //if ((GetAsyncKeyState(VK_DOWN) < 0) != keys[3]) {
        //    keys[3] = -keys[3];
        //    ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, -0.01f, 0.0f});
        //}
        //if ((GetAsyncKeyState(0x41) < 0) != keys[4]) {
        //    keys[4] = -keys[4];
        //    ip.Query<IRotatable>(cubeId)->Rotate<float>({0.0f, 0.0f, 1.0f});
        //} 
        //if ((GetAsyncKeyState(0x57) < 0) != keys[5]) {
        //    keys[5] = -keys[5];
        //    ip.Query<IRotatable>(cubeId)->Rotate<float>({1.0f, 0.0f, 0.0f});
        //}
        //if ((GetAsyncKeyState(0x44) < 0) != keys[6]) {
        //    keys[6] = -keys[6];
        //    ip.Query<IRotatable>(cubeId)->Rotate<float>({0.0f, 0.0f, -1.0f});
        //}
        //if ((GetAsyncKeyState(0x53) < 0) != keys[7]) {
        //    keys[7] = -keys[7];
        //    ip.Query<IRotatable>(cubeId)->Rotate<float>({-1.0f, -0.0f, 0.0f});
        //} 
        //if ((GetAsyncKeyState(0x46) < 0) != keys[9]) {
        //    keys[9] = -keys[9];
        //    ip.Query<ITranslatable>(cubeId)->Translate<float>({0.0f, -0.0f, 0.1f});
        //}
        //if ((GetAsyncKeyState(0x42) < 0) != keys[10]) {
        //    keys[10] = -keys[10];
        //    ip.Query<ITranslatable>(cubeId)->Translate<float>({-0.0f, -0.0f, -0.1f});
        //}
    } else if (mode == 0) {
        POINT point;
        GetCursorPos(&point);
        Vector<float> cur = {static_cast<float>(point.x), 
                             static_cast<float>(point.y)};        
        int lButtonKeyState = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        if (lButtonKeyState) {
            Matrix<float> rotation 
                = Matrix<float>::Rotation({-(cur[1] - prev[1]) / 8.0f, 
                                           (cur[0] - prev[0]) / 8.0f, 0.0f});

            dir = rotation * dir;
            camera.Rotate({-(cur[1] - prev[1]) / 8.0f, 
                           (cur[0] - prev[0]) / 8.0f, 0.0f});
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            keys[0] = -keys[0];
            Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                              90.0f, 0.0f});
            dir = rotation * dir;
            camera.Translate({-dir[0] / 10.0f, +0.0f, -dir[2] / 10.0f});
            rotation = Matrix<float>::Rotation({0.0f, -90.0f, 0.0f});  
            dir = rotation * dir;
            UpdateCamera();
        } 
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            keys[1] = -keys[1];
            Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                              -90.0f, 0.0f});
            dir = rotation * dir;
            camera.Translate({-dir[0] / 10.0f, +0.0f, -dir[2] / 10.0f});
            rotation = Matrix<float>::Rotation({0.0f, 90.0f, 0.0f});  
            dir = rotation * dir;
            UpdateCamera();
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            keys[2] = -keys[2];
            camera.Translate({dir[0] / 10.0f, +0.0f, dir[2] / 10.0f});
            UpdateCamera();
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            keys[3] = -keys[3];
            camera.Translate({-dir[0] / 10.0f, -0.0f, -dir[2] / 10.0f});
            UpdateCamera();
        }


        if (GetAsyncKeyState(0x41) & 0x8000) {
            keys[0] = -keys[0];
            Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                              90.0f, 0.0f});
            dir = rotation * dir;
            camera.Translate({-dir[0] / 10.0f, +0.0f, -dir[2] / 10.0f});
            rotation = Matrix<float>::Rotation({0.0f, -90.0f, 0.0f});  
            dir = rotation * dir;
            UpdateCamera();
        } 
        if (GetAsyncKeyState(0x44) & 0x8000) {
            keys[1] = -keys[1];
            Matrix<float> rotation = Matrix<float>::Rotation({0.0f, 
                                                              -90.0f, 0.0f});
            dir = rotation * dir;
            camera.Translate({-dir[0] / 10.0f, +0.0f, -dir[2] / 10.0f});
            rotation = Matrix<float>::Rotation({0.0f, 90.0f, 0.0f});  
            dir = rotation * dir;
            UpdateCamera();
        }
        if (GetAsyncKeyState(0x57) & 0x8000) {
            keys[2] = -keys[2];
            camera.Translate({dir[0] / 10.0f, +0.0f, dir[2] / 10.0f});
            UpdateCamera();
        }
        if (GetAsyncKeyState(0x53) & 0x8000) {
            keys[3] = -keys[3];
            camera.Translate({-dir[0] / 10.0f, -0.0f, -dir[2] / 10.0f});
            UpdateCamera();
        } 
        if ((GetAsyncKeyState(0x46) < 0) != keys[9]) {
            keys[9] = -keys[9];
            camera.Rotate({0.0f, 0.0f, 2.0f});
            UpdateCamera();

        }
        if ((GetAsyncKeyState(0x42) < 0) != keys[10]) {
            keys[10] = -keys[10];
            camera.Rotate({-0.0f, 0.0f, -2.0f});
            UpdateCamera();
        } 
        prev = cur;
    }
    if ((GetAsyncKeyState(0x4D) < 0) != keys[8]) {
        keys[8] = -keys[8];
        mode = (mode + 1) % 2;
    }
}

}
}
}
