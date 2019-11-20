#include "Scene.h"

namespace White {
namespace Engine {

Scene::Scene() {}

void Scene::AddObject(unsigned object) {
    objects.push_back(object);
}

}
}
