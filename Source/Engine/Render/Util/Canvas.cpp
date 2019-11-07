#include "Canvas.h"

namespace White {
namespace Engine {
namespace Render {
namespace Util {

Canvas::Canvas() {
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = White::Util::System::WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Canvas";
    wc.hIconSm = NULL;

    RegisterClassExW(&wc);

    hWnd = CreateWindowExW(0, L"Canvas", L"White Engine",
                           WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
                           CW_USEDEFAULT, CW_USEDEFAULT, 
                           CW_USEDEFAULT, CW_USEDEFAULT, 
                           NULL, NULL, GetModuleHandleW(NULL), NULL);
}

Canvas::~Canvas() {
    UnregisterClassW(L"Canvas", GetModuleHandleW(NULL));
}

void Canvas::show() {
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
}

void Canvas::update() {
    UpdateWindow(hWnd);
}

void Canvas::destroy() {
    DestroyWindow(hWnd);
}

void Canvas::loop() {
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

LRESULT CALLBACK Canvas::windowProcCallback(HWND hWnd, UINT uMsg,
                                            WPARAM wParam, LPARAM lParam) {   
    //switch (uMsg) {
    //case WM_CREATE:
    //    break;
    //case WM_SIZE:
    //    update();
    //    break;
    //case WM_CLOSE:
    //    destroy();
    //    break;
    //case WM_DESTROY:
    //    hglrc = wglGetCurrentContext()
    //    hdc = wglGetCurrentDC();
    //    wglMakeCurrent(NULL, NULL);
    //    ReleaseDC(hWnd, hdc);
    //    wglDeleteContext(hglrc); 
    //    PostQuitMessage(0);
    //    break;
    //default:
    //    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    //}
    
    return 0;
}

}
}
}
}
