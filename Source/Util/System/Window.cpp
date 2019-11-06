#include "Window.h"

std::vector<Window> Window::windows;

Window::Window(LPWSTR className) {
}

Window::Window(WNDCLASSEXW wc) : wc(wc) {}

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

bool Window::registerClass() {
    return RegisterClassExW(&wc);
}

bool Window::create() {
    return false;
}

bool Window::show(int nCmdShow) {
    return ShowWindow(hWnd, nCmdShow);
}

HWND Window::getHandle() const {
    return hWnd;
}
