#include "Scene.h"

namespace White {
namespace Engine {

Scene::Scene() {}

void Scene::AddObject(unsigned object) {
    ip.Add(object, objects.size());
    objects.push_back(object);
}

void Scene::RemoveObject(unsigned object) {
    unsigned index = ip.Get(object);
    ip.Remove(object);
}

}
}
