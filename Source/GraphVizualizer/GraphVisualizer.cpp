#include "GraphVisualizer.h"
#include "Renderer.h"
#include "Rectangle.h"
#include "Ring.h"
#include "Disk.h"
#include "Segment.h"
#include "Common.h"

namespace White {
namespace Engine {
namespace Graphics {

GraphVisualizer::GraphVisualizer(Renderer& renderer) 
        : Game(renderer) 
        , graph(nullptr) {}

void GraphVisualizer::LoadGraph(std::string path) {
    graph = ParseGraphFromJSON(path);
    Vector<GLfloat> cur = {-0.9f, 0.9};
    Vector<GLfloat> moveRight = {0.4, 0.0};
    Vector<GLfloat> moveDown = {0.0, -0.4};
    for (int i = 0; i < 100; i++) {
        Vertex* vertex = graph->GetVByIdx(i);
        if (vertex) {
            if (cur[0] > 0.9) {
                cur[0] = -0.9;
                cur += moveDown;
            }
            grid.push_back(cur);
            cur += moveRight;
        }
    }
    int curCell = 0;

    //for (int i = 0; i < 100; i++) {
    //    Vertex* vertex = graph->GetVByIdx(i);
    //    if (vertex) {
    //        DisplayNode(grid[curCell]);     
    //        if (curCell == 0) {
    //            DisplayEdge(grid[curCell], grid[curCell + 1]);
    //        } 
    //        curCell++;
    //    }
    //}
    for (int i = 0; i < grid.size(); i++) {
        DisplayNode(grid[i]);
    }
    for (int i = 0; i < grid.size() - 1; i++) {
        DisplayEdge(grid[i], grid[i + 1]);
    }
}

void GraphVisualizer::DisplayNode(Vector<GLfloat>& pos) {
    Vector<GLfloat> color = {1.0f, 1.0f, 0.0f, 1.0f};
    Vector<GLfloat> colorBorder = {0.0f, 0.0f, 0.5f, 1.0f};
    //Util::Math::Rectangle<GLfloat> rect(5, 0.4);
    //Mesh<GLfloat> rectMesh 
    //    = rect.ToMesh(color + Vector<GLfloat>{-0.2, -0.2, 0.3, 0.0f}, 0);
    //rectMesh.Scale({0.3f, 0.3f, 1.0f});
    //rectMesh.Translate({0.6f, 0.6f, 0.3f});
    //rectMesh.Rotate({0.0f, 0.0f, 20.0f});
    Disk<GLfloat> disk(0.2);
    Mesh<GLfloat> diskMesh = disk.ToMesh(color, 720);
    diskMesh.Scale({0.2f, 0.2f, 1.0f});
    diskMesh.Translate({pos[0], pos[1], 0.0f});
    Ring<GLfloat> ring(0.1, 0.28);
    Mesh<GLfloat> ringMesh = ring.ToMesh(colorBorder, 720);
    ringMesh.Scale({0.2f, 0.2f, 1.0f});
    ringMesh.Translate({pos[0], pos[1], 0.0f});
    //AddMesh(rectMesh);
    renderer.AddMesh(diskMesh);
    renderer.AddMesh(ringMesh);
}

void GraphVisualizer::DisplayEdge(Vector<GLfloat>& begin, 
                                  Vector<GLfloat>& end) { 
    //GLfloat halfLen = (begin - end).Length() / 2.0f;
    Vector<GLfloat> dir = end - begin;
    Vector<GLfloat> mid = begin + dir * (1.0 / 2.0f);
    Vector<GLfloat> initial = {dir.Length(), 0};
    GLfloat cross = initial.Dot(dir);
    GLfloat phi 
        = Util::Math::ToDegrees(std::acos(cross 
                                          / (dir.Length() 
                                             * initial.Length())));
    //GLfloat halfLenY = (begin[1] - end[1]) / 2.0f;
    //GLfloat halfLenX = (begin[0] - end[0]) / 2.0f;
    Vector<GLfloat> color = {0.0f, 0.0f, 0.0f, 1.0f}; 
    //Vector<GLfloat> mid(begin);
    Vector<GLfloat> rotation = {0.0f, 0.0f, -phi};
    //mid[0] += halfLenX;
    //mid[1] += halfLenY;
    Segment<GLfloat> segment(begin, end);
    Mesh<GLfloat> segmentMesh = segment.ToMesh(color, 4);
    segmentMesh.Translate({mid[0], mid[1], 0.1f});
    segmentMesh.Rotate(rotation);
    renderer.AddMesh(segmentMesh);
}

void GraphVisualizer::Play() {
    if (!graph) {
        LoadGraph("Temp/small_graph.json"); 
    }
}

}
}
}
