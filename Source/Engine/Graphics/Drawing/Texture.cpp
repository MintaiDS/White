#include "Texture.h"

namespace White {
namespace Engine {
namespace Graphics {

Texture::Texture() : image(0) {}
    
void Texture::SetImage(unsigned image) {
    this->image = image;
}

}
}
}
