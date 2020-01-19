#pragma once

#include "Game.h"
#include "Overseer.h"
#include "Vector.h"
#include "GraphView.h"
#include "Camera.h"
#include "Thread.h"

#include <string>
#include <chrono>

using namespace White::Util::Math;
using namespace Threading;

namespace White {
namespace Engine {
namespace Graphics {

class GraphVisualizer : public Game {
public:
    GraphVisualizer(Renderer& renderer);

    DWORD Listener();
    DWORD ViewInitializer();

    void LoadGraph(std::string path);
    void UpdateCamera();

    virtual void Play();

protected:
    Thread<GraphVisualizer> overseerThread;
    Thread<GraphVisualizer> initializerThread; 
    Vector<float> prev;
    Vector<float> dir;
    Vector<float> rotation;
    int mode = 0;
    int keys[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    float scaleFactor;
    unsigned cubeId;
    float cameraScalingStep;
    float cameraScaling;
    Vector<float> cameraTranslation;
    Camera<float> camera;
    std::shared_ptr<Overseer> overseer;
    std::chrono::milliseconds prevTime;
    std::shared_ptr<Graph> graph;
    std::shared_ptr<Grid> grid;
    GraphView graphView;
};

}
}
}
