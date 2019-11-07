#include <string>
#include <sstream>
#include <random>

#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "Canvas.h"

namespace White {
namespace Engine {
namespace Render {
namespace Util {

Canvas::Canvas(HINSTANCE hInstance) {
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = static_cast<WNDPROC>(White::Util::System::WindowProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Canvas";
    wc.hIconSm = NULL;

    if (!RegisterClassExW(&wc)) {
        MessageBoxW(NULL, L"register failed", NULL, MB_OK);
    }

    hWnd = CreateWindowExW(0, L"Canvas", L"White Engine",
                           WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
                           CW_USEDEFAULT, CW_USEDEFAULT, 
                           CW_USEDEFAULT, CW_USEDEFAULT, 
                           NULL, NULL, hInstance, NULL);

    DWORD x = GetLastError();
    wchar_t err[512];
    swprintf(err, L"%d", hWnd);
    
    if (hWnd == NULL) {
        MessageBoxW(NULL, err, NULL, MB_OK);
    }
 
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1, 
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        24,
        0, 0, 0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        32, 0, 0,
        PFD_MAIN_PLANE,
        0, 
        0, 0, 0
    };
    HDC hdc = GetDC(hWnd);
    int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, iPixelFormat, &pfd);   
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);     
    glewInit(); 
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
    HDC hdc;
    HGLRC hglrc;
    std::random_device random;
    GLfloat r;
    GLfloat g;
    GLfloat b;
    switch (uMsg) {
    case WM_PAINT:
        hdc = GetDC(hWnd);
        glClear(GL_COLOR_BUFFER_BIT); 
        r = (random() % 1000000000) / 1000000000.0;
        g = (random() % 1000000000) / 1000000000.0;
        b = (random() % 1000000000) / 1000000000.0;
        glClearColor(r, g, b, 1.0f);
        SwapBuffers(hdc);
        break;
    case WM_CREATE:
        MessageBoxW(NULL, L"On canvas create", NULL, MB_OK);
        break;
    case WM_SIZE:
        update();
        break;
    case WM_CLOSE:
        destroy();
        break;
    case WM_DESTROY:
        hglrc = wglGetCurrentContext();
        hdc = wglGetCurrentDC();
        wglMakeCurrent(NULL, NULL);
        ReleaseDC(hWnd, hdc);
        wglDeleteContext(hglrc); 
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

}
}
}
}
