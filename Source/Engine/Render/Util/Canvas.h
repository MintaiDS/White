#pragma once

#include "Window.h"

namespace White {
namespace Engine {
namespace Render {
namespace Util {

class Canvas : public White::Util::System::Window {
public:
    Canvas();

    virtual LRESULT CALLBACK windowProcCallback(HWND hWnd, UINT uMsg, 
                                                 WPARAM wParam, LPARAM lParam);
};

}
}
}
}
