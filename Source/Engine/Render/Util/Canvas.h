#pragma once

#include "Window.h"

using namespace White::Util::System;

namespace White {
namespace Engine {
namespace Render {
namespace Util {

class Canvas : public Window {
public:
    Canvas(HINSTANCE hInstance);
    ~Canvas();

    void Show();
    void Update();
    void Destroy();
    void Loop();
    void Render();

    virtual LRESULT CALLBACK WindowProcCallback(HWND hWnd, UINT uMsg, 
                                                 WPARAM wParam, LPARAM lParam);
};

}
}
}
}
