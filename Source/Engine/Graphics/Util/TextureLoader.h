#pragma once

#include "BMPLoader.h"
#include "Texture.h"

namespace White {
namespace Engine {
namespace Graphics {

class TextureLoader {
public:
    TextureLoader(std::wstring dir = L""); 

    Texture Import(std::wstring filename);
    void SetDirectory(std::wstring dir);
    Texture GetTexture();

protected:
    Texture texture;
    std::wstring dir;
    BMPLoader bitmapLoader;
};

}
}
}
