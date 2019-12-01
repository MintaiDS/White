#include "Object.h"
#include "ObjectManager.h"

namespace White {

Object::~Object() {}

void Object::AddChild(unsigned child) {
    childs.push_back(child);
}

}
