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

    void show();
    void update();
    void destroy();
    void loop();
    void render();

    virtual LRESULT CALLBACK windowProcCallback(HWND hWnd, UINT uMsg, 
                                                 WPARAM wParam, LPARAM lParam);
};

}
}
}
}
