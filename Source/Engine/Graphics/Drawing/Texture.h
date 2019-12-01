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
    
    void Create();
    void Delete();
    void Bind();
    void SetImage(BMPImage image);
    unsigned char* GetDataPtr();
    bool IsBinded() const;

protected:
    bool isBinded;
    unsigned id;
    BMPImage image;
};

}
}
}
