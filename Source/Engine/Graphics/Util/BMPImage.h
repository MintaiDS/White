#pragma once

#include <memory>

namespace White {
namespace Engine {
namespace Util {

class BMPImage {
public:
    BMPImage();
    BMPImage(const BMPImage& other);

    void SetData(int size, const std::unique_ptr<char[]>& data); 
    void SetWidth(int width);
    void SetHeight(int height);
    int GetWidth() const;
    int GetHeight() const;
    int GetSize() const;
    template<typename T>
    T* GetDataPtr() const;

protected:
    int width;
    int height;
    std::unique_ptr<char[]> data;
};

}
}
}
