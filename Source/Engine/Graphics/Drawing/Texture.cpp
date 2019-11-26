#include "Texture.h"

namespace White {
namespace Engine {
namespace Graphics {

Texture::Texture() {}

Texture::Texture(const Texture& other) : image(other.image) {}
    
void Texture::SetImage(BMPImage image) {
    this->image = image;
}

}
}
}
