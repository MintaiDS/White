#include "Window.h"

namespace Util {
namespace System {

std::vector<Window> Window::windows;

LRESULT CALLBACK Window::windowProc(HWND hWnd, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam) {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i].hWnd == hWnd) {
            return windows[i].windowProcCallback(hWnd, uMsg, wParam, lParam);
        }
    }

    return 0;
}

LRESULT CALLBACK Window::windowProcCallback(HWND hWnd, UINT uMsg, 
                                            WPARAM wParam, LPARAM lParam) {
    return 0; 
}

}
}
