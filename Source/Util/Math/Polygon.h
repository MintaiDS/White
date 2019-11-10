#pragma once

#include "Vector.h"
#include "Ellipse.h"
#include "Disk.h"
#include "Ring.h"
#include "Rectangle.h"
#include "Segment.h"

namespace White {
namespace Util {
namespace Math {

template<typename T>
struct Polygon {
    Polygon(const std::vector<Vector<T>>& vertices);

    static Polygon<T> CreateFromShape(
                                const Ellipse<T>& ellipse, int verticesCnt);
    static Polygon<T> CreateFromShape(
                                const Disk<T>& disk, int verticesCnt);
    static Polygon<T> CreateFromShape(
                                const Ring<T>& ring, int verticesCnt);
    static Polygon<T> CreateFromShape(const Rectangle<T>& rectangle);
    static Polygon<T> CreateFromShape(const Segment<T>& segment);

    void Add(const Vector<T>& vertex);

    std::vector<Vector<T>> vertices;
};

template<typename T>
Polygon<T> Polygon<T>::CreateFromShape(
                                const Ellipse<T>& ellipse, int verticesCnt) {
    std::vector<Vector<T>> vertices;
    T angle = 0;
    Vector<T> vertex = {ellipse.w / 2.0, 0};
    for (int i = 0; i < verticesCnt; i++) {
        vertices.push_back(vertex);
        angle += 360.0 / verticesCnt;
        vertex = {(ellipse.w / 2.0) * std::cos(ToRadians(angle)), 
                  (ellipse.h / 2.0) * std::sin(ToRadians(angle))};
    }
    Polygon<T> ret(vertices);

    return ret;
}

template<typename T>
Polygon<T> Polygon<T>::CreateFromShape(
                                const Disk<T>& disk, int verticesCnt) {
    std::vector<Vector<T>> vertices;
    T angle = 0;
    Vector<T> vertex = {disk.w / 2.0, 0};
    for (int i = 1; i < verticesCnt; i++) {
        vertices.push_back(vertex);
        angle += 360.0 / verticesCnt;
        vertex = {(disk.w / 2.0) * std::cos(ToRadians(angle)), 
                  (disk.h / 2.0) * std::sin(ToRadians(angle))};
    }
    Polygon<T> ret(vertices);

    return ret;
}

template<typename T>
Polygon<T> Polygon<T>::CreateFromShape(
                                const Ring<T>& ring, int verticesCnt) {
    std::vector<Vector<T>> vertices;
    T angle = 0;
    Vector<T> vertexInner = {ring.inner.w / 2.0, 0};
    Vector<T> vertexOuter = {ring.outer.w / 2.0, 0}
    for (int i = 1; i < verticesCnt; i++) {
        vertices.push_back(vertexInner);
        vertices.push_back(vertexOuter);
        angle += 360.0 / verticesCnt;
        vertexInner = {(ring.inner.w / 2.0) * std::cos(ToRadians(angle)), 
                       (ring.inner.h / 2.0) * std::sin(ToRadians(angle))};
        vertexOuter = {(ring.outer.w / 2.0) * std::cos(ToRadians(angle)), 
                       (ring.outer.h / 2.0) * std::sin(ToRadians(angle))};
    }
    Polygon<T> ret(vertices);

    return ret;
}

template<typename T>
Polygon<T> Polygon<T>::CreateFromShape(const Rectangle<T>& rectangle) {
    std::vector<Vector<T>> vertices;
    vertices.push_back({-rect.w / 2.0, -rect.h / 2.0});
    vertices.push_back({-rect.w / 2.0, rect.h / 2.0});
    vertices.push_back({rect.w / 2.0, rect.h / 2.0});
    vertices.push_back({rect.w / 2.0, -rect.h / 2.0});
    Polygon<T> ret(vertices);

    return ret;
}

template<typename T>
Polygon<T> Polygon<T>::CreateFromShape(
                                const Segment<T>& segment) {
    std::vector<Vector<T>> vertices; 
    Polygon<T> ret(vertices);

    return ret;
}

template<typename T>
Polygon<T>::Polygon(const std::vector<Vector<T>>& vertices) 
        : vertices(vertices) {}

template<typename T>
void Polygon<T>::Add(const Vector<T>& vertex) {
    vertices.push_back(vertex);
}

}
}
}
