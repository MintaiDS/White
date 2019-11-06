#include <vector>

#include "windows.h"

class Window {
public:
    Window();
    Window(LPWSTR className);
    Window(WNDCLASSEXW wc);
     
    static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, 
                                       WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK windowProcCallback(HWND hWnd, UINT uMsg,
                                        WPARAM wParam, LPARAM lParam);

    bool registerClass();
    bool create();
    bool show(int nCmdShow = SW_SHOW);
    HWND getHandle() const;

protected:
    HWND hWnd;
    WNDCLASSEXW wc;

public:
    static std::vector<Window> windows; 
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
                            WPARAM wParam, LPARAM lParam) {
    return Window::windowProc(hWnd, uMsg, wParam, lParam);
} 
