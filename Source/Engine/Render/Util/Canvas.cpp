#include "Canvas.h"
#include "GLFunctions.h"
#include "GLInitializer.h"
#include "Program.h"

#include <random>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

using namespace White::Engine::Render::GL;

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
    GLInitializer::Init(); 
    Program program;
    Shader shader(GL_VERTEX_SHADER);
    std::wstring path = L"../../../../Engine/Shaders/default.vsh";
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    shader.Delete();
    path = L"../../../../Engine/Shaders/default.fsh";
    shader.Create(GL_FRAGMENT_SHADER);
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    shader.Delete();
    program.Link();
    program.Use();
    program.Delete();
}

Canvas::~Canvas() {
    UnregisterClassW(L"Canvas", GetModuleHandleW(NULL));
}

void Canvas::Show() {
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
}

void Canvas::Update() {
    UpdateWindow(hWnd);
}

void Canvas::Destroy() {
    DestroyWindow(hWnd);
}

void Canvas::Loop() {
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
        Render();
    }  
}

void Canvas::Render() {
    std::random_device random;
    GLfloat r = (random() % 1000) / 1000.0;
    GLfloat g = (random() % 1000) / 1000.0;
    GLfloat b = (random() % 1000) / 1000.0;
    HDC hdc = GetDC(hWnd);
    GLInitializer::Init();
    GLFunctions::Get().CreateProgram();
    GLFunctions::Get().Clear(GL_COLOR_BUFFER_BIT);
    GLFunctions::Get().ClearColor(r, g, b, 1.0f);
    Sleep(33);
    SwapBuffers(hdc);
}

LRESULT CALLBACK Canvas::WindowProcCallback(HWND hWnd, UINT uMsg,
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
        Update();
        break;
    case WM_CLOSE:
        Destroy();
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
