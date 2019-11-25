#include "BMPLoader.h"

#include <windows.h>
#include <winuser.h>
#include <wingdi.h>

#include <memory>

namespace White {
namespace Engine {
namespace Util {

BMPLoader::BMPLoader() {
}

void BMPLoader::Import(std::wstring filename) {
    HDC hdc;
    HBITMAP hBitmap;
    BITMAPINFO bmi;
    std::unique_ptr<char[]> buffer;
    hdc = GetDC(nullptr);
    hBitmap = static_cast<HBITMAP>(LoadImageW(nullptr, 
                                              filename.c_str(), 
                                              IMAGE_BITMAP, 0, 0, 
                                              LR_CREATEDIBSECTION  
                                              | LR_LOADFROMFILE));
    GetDIBits(hdc, hBitmap, 0, 0, NULL, &bmi, DIB_RGB_COLORS);
    buffer = std::make_unique<char[]>(bmi.bmiHeader.biSizeImage);
    bmi.bmiHeader.biCompression = BI_RGB;
    GetDIBits(hdc, hBitmap, 0, bmi.bmiHeader.biHeight, 
              static_cast<void*>(buffer.get()), &bmi, DIB_RGB_COLORS);
    image.SetData(bmi.bmiHeader.biSizeImage, buffer);
    image.SetWidth(bmi.bmiHeader.biWidth);
    image.SetHeight(bmi.bmiHeader.biHeight);
    DeleteObject(hBitmap);
    ReleaseDC(nullptr, hdc);
}

void BMPLoader::Export(std::wstring filename) {
}

BMPImage BMPLoader::GetImage() {
    return image;
}


}
}
}
