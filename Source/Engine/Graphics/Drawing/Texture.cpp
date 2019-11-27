#include "Texture.h"

#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Graphics {

Texture::Texture() {}

Texture::Texture(const Texture& other) : id(0), image(other.image) {}

void Texture::Create() {
    glGenTextures(1, &id);
}

void Texture::Delete() {
    glDeleteTextures(1, &id);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    width = image.GetWidth();
    height = image.GetHeight();
    unsigned char* data = image.GetDataPtr<unsigned char>();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                 width, height,  0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
    
void Texture::SetImage(BMPImage image) {
    this->image = image;
}

unsigned char* Texture::GetDataPtr() {
    return image.GetDataPtr<unsigned char>();
}

}
}
}
