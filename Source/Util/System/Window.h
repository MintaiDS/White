#include <vector>

#include "windows.h"

namespace Util {
namespace System {

class Window {
public:    
    static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, 
                                       WPARAM wParam, LPARAM lParam);
    virtual LRESULT CALLBACK windowProcCallback(HWND hWnd, UINT uMsg,
                                        WPARAM wParam, LPARAM lParam);
protected:
    HWND hWnd;
    WNDCLASSEXW wc;
    static std::vector<Window> windows; 
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
                            WPARAM wParam, LPARAM lParam) {
    return Window::windowProc(hWnd, uMsg, wParam, lParam);
} 

}
}
