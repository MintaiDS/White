#pragma once

#include <memory>

namespace White {
namespace Engine {
namespace Util {

class BMPImage {
public:
    BMPImage();
    BMPImage(const BMPImage& other);

    BMPImage& operator=(const BMPImage& other);
    void SetData(int size, const std::unique_ptr<unsigned char[]>& data); 
    void SetWidth(int width);
    void SetHeight(int height);
    void SetSize(int size);
    int GetWidth() const;
    int GetHeight() const;
    int GetSize() const;
    template<typename T>
    T* GetDataPtr() const;

protected:
    int width;
    int height;
    int size;
    std::unique_ptr<unsigned char[]> data;
};

template<typename T>
T* BMPImage::GetDataPtr() const {
    return reinterpret_cast<T*>(data.get());
}

}
}
}
