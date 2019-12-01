#pragma once

#include <deque>

namespace White {
namespace Engine {

template<typename T>
class ObjectIDProvider {
public:
    ObjectIDProvider();

    T GetNextID();
    void Free(T id);

protected:
    T lastID;
    std::deque<T> freeIDs;
};

template<typename T>
ObjectIDProvider<T>::ObjectIDProvider() : lastID(0) {}

template<typename T>
T ObjectIDProvider<T>::GetNextID() {
    if (!freeIDs.empty()) {
        T ret = freeIDs.front();
        freeIDs.pop_front();

        return ret;
    }

    return ++lastID;
}

template<typename T>
void ObjectIDProvider<T>::Free(T id) {
    freeIDs.push_back(id);
}

}
}
