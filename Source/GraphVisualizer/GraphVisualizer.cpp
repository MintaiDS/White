#include "GraphVisualizer.h"
#include "StartupSettings.h"
#include "Renderer.h"
#include "Rectangle.h"
#include "Ring.h"
#include "Disk.h"
#include "Segment.h"
#include "Common.h"
#include "Grid.h"
#include "Connection.h"
#include "Camera.h"

#include <AtlBase.h>
#include <atlconv.h>

#include <sstream>
#include <fstream>
#include <iostream>

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
   /* data = "{\"layer\":10}";
    msg = conn.FormActionMessage(Action::MAP, data);
    conn.Request(msg, resp);
    delete[](resp.data);
    if (resp.result == Result::OKEY)
    {
      ParseCoordFromJSON(gr, resp.data);
    }*/
    //msg = conn.FormActionMessage(Action::LOGOUT, "");
    //conn.Request(msg, resp);
    //graph.reset(&gr);
}

void GraphVisualizer::UpdateCamera() {
    Program& program = renderer.GetProgram();

    // Setup view matrix.
    //Matrix<GLfloat> view = Matrix<GLfloat>::Identity(2);
    //view[0][0] *= (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
    //view[1][1] *= (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
    //view.Inverse();
    //view = {{view[0][0], view[0][1], 0.0f, 0.0f}, 
    //        {view[1][0], view[1][1], 0.0f, 0.0f}, 
    //        {0.0f, 0.0f, 1.0f, 0.0f}, 
    //        {0.0f, 0.0f, 0.0f, 1.0f}};
    //camera.Translate({0.0f, 0.0f, -0.1f});
    //camera.Rotate({0.0f, 0.0f, -20.0f});
    //camera.Scale({1.1f, 1.1f, 1.1f});
    Matrix<GLfloat> view = camera.GetViewMatrix();
    std::unique_ptr<GLfloat[]> raw 
        = std::make_unique<GLfloat[]>(view.rows * view.columns);
    program.Use();
    GLint location = glGetUniformLocation(program.id, "view");
    for (int i = 0; i < view.rows; i++) {
        for (int j = 0; j < view.columns; j++) {
            raw.get()[i * view.columns + j] = view[i][j];
        }
    }
    program.Use();
    glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());

    GLfloat scaleFactor = (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;

    // Setup projection matrix.
    //Matrix<GLfloat> projection = Matrix<GLfloat>::Identity(4);
    Matrix<GLfloat> projection
        = Matrix<GLfloat>::Projection(-1.0f, 1.0f, 
                                      1.0f, -1.0f, 0.1f, 10.0f);
    //projection *= -1;
    for (int i = 0; i < projection.rows; i++) {
        for (int j = 0; j < projection.columns; j++) {
            raw.get()[i * projection.columns + j] = projection[i][j];
        }
    }
    location = glGetUniformLocation(program.id, "projection");
    program.Use();
    glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
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
        //int dimension = (std::log2(verticesCnt) - 1) 
        //                * (std::sqrt(verticesCnt) + 1);
        int dimension = 2 * (std::sqrt(verticesCnt) + 1); 
        grid.reset(new Grid({0.0f, 0.0f}, 
                            {dimension, dimension}, 
                            {0.25f, 0.25f}));
        UpdateCamera();
        graphView.SetRenderer(&renderer);
        graphView.SetGraph(graph);
        graphView.SetGrid(grid);
        graphView.Init();
        graphView.Display();
        cameraScaling = 1.0f;
        cameraScalingStep = 0.03f;
        cameraTranslation = {0, 0, 0};
        //camera.Translate({0.0f, 0.0f, -0.5f});
        camera.Scale({1.0f, 1.0f, 1.0f});
        GLfloat scaleFactor = (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;

        camera.Scale({scaleFactor, scaleFactor, scaleFactor});

        camera.Translate({0.0f, 0.0f, 1.0f});
        //camera.Translate({0.0f, 0.0f, (grid->gridSize[1] * grid->cellSize[0]) / 2.0f});
        //camera.Scale({1.8f, 1.8f, 1.8f});
        //glDisable(GL_CULL_FACE);
    
    }

    Program& program = renderer.GetProgram();

    cameraScaling += cameraScalingStep;
    if (std::abs(cameraScaling) >= 1.1f) {
        cameraScalingStep = -cameraScalingStep;
    }
    camera.Rotate({0.0f, 3.0f, 0.0f});
    //camera.Translate(cameraTranslation);
    //camera.Scale({std::abs(cameraScaling), std::abs(cameraScaling), 1.0f});
    //camera.Translate(
    Matrix<GLfloat> view = camera.GetViewMatrix();
    std::unique_ptr<GLfloat[]> raw 
        = std::make_unique<GLfloat[]>(view.rows * view.columns);
    program.Use();
    GLint location = glGetUniformLocation(program.id, "view");
    for (int i = 0; i < view.rows; i++) {
        for (int j = 0; j < view.columns; j++) {
            raw.get()[i * view.columns + j] = view[i][j];
        }
    }
    program.Use();
    glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());

}

}
}
}
