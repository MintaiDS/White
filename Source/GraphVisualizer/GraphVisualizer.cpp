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
    Matrix<GLfloat> view = Matrix<GLfloat>::Identity(2);
    view[0][0] *= (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
    view[1][1] *= (grid->gridSize[1] * grid->cellSize[0]) / 2.0f;
    view.Inverse();
    view = {{view[0][0], view[0][1], 0.0f, 0.0f}, 
            {view[1][0], view[1][1], 0.0f, 0.0f}, 
            {0.0f, 0.0f, 1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f, 1.0f}};
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
        int dimension = std::sqrt(verticesCnt) + 1;
        grid.reset(new Grid({0.0f, 0.0f}, 
                            {dimension, dimension}, 
                            {0.25f, 0.25f}));
        UpdateCamera();
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
