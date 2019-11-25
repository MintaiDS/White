#include "BMPImage.h"

namespace White {
namespace Engine {
namespace Util {

BMPImage::BMPImage() : width(0), height(0) {}

BMPImage::BMPImage(const BMPImage& other) 
        : width(other.width)
        , height(other.height) {
    data = std::make_unique<char[]>(width * height);
    std::copy(other.data.get(), other.data.get() + width * height, data.get());
}

void BMPImage::SetData(int size, const std::unique_ptr<char[]>& data) {
    this->data = std::make_unique<char[]>(size);
    std::copy(data.get(), data.get() + size, this->data.get());
}

void BMPImage::SetWidth(int width) {
    this->width = width;
}

void BMPImage::SetHeight(int height) {
    this->height = height;
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

template<typename T>
T* BMPImage::GetDataPtr() const {
    return reinterpret_cast<T*>(data.get());
}

}
}
}
