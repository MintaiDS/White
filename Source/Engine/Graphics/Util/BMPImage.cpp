#include "BMPImage.h"
#include "Logger.h"

namespace White {
namespace Engine {
namespace Util {

BMPImage::BMPImage() : width(0), height(0), size(0) {}

BMPImage::BMPImage(const BMPImage& other) 
        : width(other.width)
        , height(other.height)
        , size(other.size) {
    data = std::make_unique<unsigned char[]>(size);
    std::copy(other.data.get(), other.data.get() + size, data.get());
}

BMPImage& BMPImage::operator=(const BMPImage& other) {
    width = other.width;
    height = other.height;
    size = other.size;
    data = std::make_unique<unsigned char[]>(size);
    std::copy(other.data.get(), other.data.get() + size, data.get());

    return *this;
}

void BMPImage::SetData(int size, const std::unique_ptr<unsigned char[]>& data) {
    this->data = std::make_unique<unsigned char[]>(size);
    std::copy(data.get(), data.get() + size, this->data.get());
}

void BMPImage::SetWidth(int width) {
    this->width = width;
}

void BMPImage::SetHeight(int height) {
    this->height = height;
}

void BMPImage::SetSize(int size) {
    this->size = size;
}

int BMPImage::GetWidth() const {
    return width;
}

int BMPImage::GetHeight() const {
    return height;
}

int BMPImage::GetSize() const {
    return width * height;
}

}
}
}
