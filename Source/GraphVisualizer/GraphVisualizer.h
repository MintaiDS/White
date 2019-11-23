#pragma once

#include "Game.h"
#include "Graph.h"
#include "Vector.h"
#include "GraphView.h"
#include "Camera.h"

#include <string>

using namespace White::Util::Math;

namespace White {
namespace Engine {
namespace Graphics {

class GraphVisualizer : Game {
public:
    GraphVisualizer(Renderer& renderer);

    void LoadGraph(std::string path);
    void UpdateCamera();

    virtual void Play();

protected:
    int mode = 1;
    bool keys[9] = {false, false, false, false, false, false, false, false, false};
    unsigned cubeId;
    float cameraScalingStep;
    float cameraScaling;
    Vector<float> cameraTranslation;
    Camera<float> camera;
    std::shared_ptr<Graph> graph;
    std::shared_ptr<Grid> grid;
    GraphView graphView;
};

}
}
}
