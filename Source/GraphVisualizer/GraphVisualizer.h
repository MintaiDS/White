#pragma once

#include "Game.h"
#include "Overseer.h"
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
    Vector<float> prev;
    Vector<float> dir;
    int mode = 0;
    int keys[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    float scaleFactor;
    unsigned cubeId;
    float cameraScalingStep;
    float cameraScaling;
    Vector<float> cameraTranslation;
    Camera<float> camera;

    std::shared_ptr<Overseer> overseer;

    std::shared_ptr<Graph> graph;
    std::shared_ptr<Grid> grid;
    GraphView graphView;
};

}
}
}
