#include "TextureLoader.h"

namespace White {
namespace Engine {
namespace Graphics {

TextureLoader::TextureLoader(std::wstring dir) : dir(dir) {}

Texture TextureLoader::Import(std::wstring filename) {
    bitmapLoader.Import(dir + filename);
    texture.SetImage(bitmapLoader.GetImage());

    return texture;
}

void TextureLoader::SetDirectory(std::wstring dir) {
    this->dir = dir;
}

Texture TextureLoader::GetTexture() {
    return texture;
}

}
}
}
