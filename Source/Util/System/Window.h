#pragma once

#include <windows.h>

#include <vector>

namespace White {
namespace Util {
namespace System {

class Window {
public:    
    Window();

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
                                       WPARAM wParam, LPARAM lParam);
    virtual LRESULT CALLBACK WindowProcCallback(HWND hWnd, UINT uMsg,
                                                WPARAM wParam, LPARAM lParam);

protected:
    HWND hWnd;
    WNDCLASSEXW wc;
    static std::vector<Window*> windows; 
};

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
                                   WPARAM wParam, LPARAM lParam) {
    return Window::WindowProc(hWnd, uMsg, wParam, lParam);
} 

}
}
}
