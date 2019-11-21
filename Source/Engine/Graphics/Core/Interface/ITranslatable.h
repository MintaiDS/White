#pragma once

#include "Vector.h"

using namespace White::Util::Math;

namespace White {
namespace Engine {

class ITranslatable {
public:
    virtual ~ITranslatable();
    
    template<typename T>
    void Translate(const Vector<T>& translation);
    
private:
    virtual void Translate(const Vector<float>& translation) = 0;
};

template<typename T>
void ITranslatable::Translate(const Vector<T>& translation) {
    Translate(translation);
}

}
}
