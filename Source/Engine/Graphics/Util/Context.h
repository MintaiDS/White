#pragma once

#include "Window.h"
#include "Program.h"

using namespace White::Util::System;

namespace White {
namespace Engine {
namespace Graphics {
namespace Util {

class Context : public Window {
public:
    Context();
    ~Context();

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
