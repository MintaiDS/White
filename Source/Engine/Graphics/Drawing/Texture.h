#pragma once

#include "BMPImage.h"
#include "Object.h"

using namespace White::Engine::Util;

namespace White {
namespace Engine {
namespace Graphics {

class Texture : public Object {
public:
    Texture();
    Texture(const Texture& other);
    
    void SetImage(BMPImage image);

protected:
    BMPImage image;
};

}
}
}
