#include "Window.h"

namespace White {
namespace Util {
namespace System {

std::vector<Window*> Window::windows;

Window::Window() {
    windows.push_back(this);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam) {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i]->hWnd == hWnd) {
            return windows[i]->WindowProcCallback(hWnd, uMsg, wParam, lParam);
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::WindowProcCallback(HWND hWnd, UINT uMsg, 
                                            WPARAM wParam, LPARAM lParam) {
    MessageBox(NULL, NULL, NULL, MB_OK);
    return DefWindowProcW(hWnd, uMsg, wParam, lParam); 
}

}
}
}
