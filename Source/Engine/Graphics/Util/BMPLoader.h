#pragma once

#include "BMPImage.h"

#include <string>

namespace White {
namespace Engine {
namespace Util {

class BMPLoader {
public:
    BMPLoader();

    void Import(std::wstring filename);
    void Export(std::wstring filename);
    BMPImage GetImage();

protected:
    BMPImage image;
};

}
}
}
