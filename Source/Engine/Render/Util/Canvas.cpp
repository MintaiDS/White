#include "Canvas.h"

#include <string>
#include <sstream>
#include <random>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

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
    while (true) { 
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) { 
            if (GetMessageW(&msg, NULL, 0, 0) ) { 
                TranslateMessage(&msg); 
                DispatchMessageW(&msg); 
            } else { 
                return;
            } 
        } 
        render();
    }  
}

void Canvas::render() {
    GLfloat r = 1.0f;
    GLfloat g = 0.5f;
    GLfloat b = 0.5f;
    PFNGLCLEARPROC glClear;
    PFNGLCLEARCOLORPROC glClearColor;
    HDC hdc = GetDC(hWnd);
    HMODULE hModule = LoadLibraryW(L"opengl32.dll");
    glClear = reinterpret_cast<PFNGLCLEARPROC>
              (GetProcAddress(hModule, "glClear"));
    glClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>
                   (GetProcAddress(hModule, "glClearColor"));
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(r, g, b, 1.0f);
    SwapBuffers(hdc);
}

LRESULT CALLBACK Canvas::windowProcCallback(HWND hWnd, UINT uMsg,
                                            WPARAM wParam, LPARAM lParam) {   
    HDC hdc;
    HGLRC hglrc; 

    switch (uMsg) {
    case WM_PAINT:
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
