#pragma once

#include "Window.h"
#include "Program.h"
#include "Core.h"

using namespace White::Util::System;

namespace White {
namespace Engine {
namespace Graphics {

class Context : public Window {
public:
    Context(Core& core);
    ~Context();

    void Init();
    void CreateWindowClass();
    void RegisterWindowClass();
    void CreateContextWindow();
    void SetPixelFormatDescriptor();
    void Create();
    void SetupDemo();
    void Show();
    void Update();
    void Destroy();
    void Loop();
    void Render();

    virtual LRESULT CALLBACK WindowProcCallback(HWND hWnd, UINT uMsg, 
                                                WPARAM wParam, LPARAM lParam);

protected:
    PIXELFORMATDESCRIPTOR pfd;
    Core& core;
};

}
}
}
