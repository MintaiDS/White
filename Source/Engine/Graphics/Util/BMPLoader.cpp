#include "BMPLoader.h"
#include "Logger.h"

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
    HDC hMemDC;
    HBITMAP hBitmap;
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof bmi.bmiHeader;
    std::unique_ptr<unsigned char[]> buffer;
    hMemDC = CreateCompatibleDC(nullptr);
    hBitmap = static_cast<HBITMAP>(LoadImageW(nullptr, 
                                              filename.c_str(), 
                                              IMAGE_BITMAP, 0, 0,   
                                              LR_LOADFROMFILE));
    SelectObject(hMemDC, hBitmap);
    GetDIBits(hMemDC, hBitmap, 0, 0, nullptr, &bmi, DIB_RGB_COLORS);
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    int lineSize = (bmi.bmiHeader.biBitCount * bmi.bmiHeader.biWidth + 31) 
                   / 32;
    int bufferSize = lineSize * 4 * bmi.bmiHeader.biHeight;
    int bytesPerLine = (bmi.bmiHeader.biWidth * bmi.bmiHeader.biBitCount) / 8;
    buffer = std::make_unique<unsigned char[]>(bufferSize);
    GetDIBits(hMemDC, hBitmap, 0, bmi.bmiHeader.biHeight, 
              static_cast<void*>(buffer.get()), &bmi, DIB_RGB_COLORS);
    int insertAt = 0;
    for (int i = 0; i < bmi.bmiHeader.biHeight; i++) {
        for (int j = 0; j < bytesPerLine; j++) {
            buffer[insertAt++] = buffer[i * lineSize * 4 + j];
        }
    }
    image.SetData(bytesPerLine * bmi.bmiHeader.biHeight, buffer);
    image.SetWidth(bmi.bmiHeader.biWidth);
    image.SetHeight(bmi.bmiHeader.biHeight);
    image.SetSize(bytesPerLine * bmi.bmiHeader.biHeight);
    DeleteObject(hBitmap);
    ReleaseDC(nullptr, hMemDC); 
}

void BMPLoader::Export(std::wstring filename) {
}

BMPImage BMPLoader::GetImage() {
    return image;
}


}
}
}
