#include "Context.h"
#include "GLFunctions.h"
#include "GLInitializer.h"
#include "VertexArrayObject.h"
#include "Program.h"
#include "BufferObject.h"
#include "VertexAttribute.h"
#include "VertexData.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include "Disk.h"
#include "Ring.h"
#include "StartupSettings.h"

#include <random>
#include <sstream>
#include <fstream>
#include <string>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

using namespace White::Engine::Graphics;

namespace White {
namespace Engine {
namespace Graphics {

Context::Context() {}

Context::~Context() {
    UnregisterClassW(L"Context", GetModuleHandleW(NULL));
}

void Context::Init() {
    CreateWindowClass();
    RegisterWindowClass(); 
    CreateContextWindow();
    SetPixelFormatDescriptor();
    Create();
}

void Context::CreateWindowClass() {
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = static_cast<WNDPROC>(White::Util::System::WindowProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Context";
    wc.hIconSm = NULL;
}

void Context::RegisterWindowClass() {
    RegisterClassExW(&wc);
}

void Context::CreateContextWindow() {
    hWnd = CreateWindowExW(0, L"Context", L"White Engine",
                           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                           10, 10,  
                           760, 760, 
                           NULL, NULL, GetModuleHandleW(NULL), NULL);
    StartupSettings::GetInstance().SetWindowHandle(hWnd);
}

void Context::SetPixelFormatDescriptor() {
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;                                                           
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24; 
    pfd.cRedBits = 0; 
    pfd.cRedShift = 0; 
    pfd.cGreenBits = 0; 
    pfd.cGreenShift = 0; 
    pfd.cBlueBits = 0;
    pfd.cBlueShift = 0; 
    pfd.cAlphaBits = 0; 
    pfd.cAlphaShift = 0;
    pfd.cAccumBits = 0; 
    pfd.cAccumRedBits = 0; 
    pfd.cAccumGreenBits = 0;  
    pfd.cAccumBlueBits = 32;
    pfd.cAccumAlphaBits = 0;  
    pfd.cDepthBits = 0;
    pfd.cStencilBits = PFD_MAIN_PLANE;
    pfd.cAuxBuffers = 0;
    pfd.iLayerType = 0;
    pfd.bReserved = 0;
    pfd.dwLayerMask = 0;  
}

void Context::Create() {
    HDC hdc = GetDC(hWnd);
    int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, iPixelFormat, &pfd);   
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);     
    LoadFunctions();
}

void Context::SetupDemo(Core& core) {
    core.renderer.Init();
}

void Context::Show() {
    ShowWindow(hWnd, SW_SHOW);
}

void Context::Update() {
    UpdateWindow(hWnd);
}

void Context::Destroy() {
    DestroyWindow(hWnd);
}


void Context::Loop(Core& core) {
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
        core.renderer.Render();
        SwapBuffers(GetDC(hWnd));
        //Sleep(100);
    }  
}

void Context::Render() {
}

LRESULT CALLBACK Context::WindowProcCallback(HWND hWnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam) {   
    HDC hdc;
    HGLRC hglrc; 

    switch (uMsg) {
    case WM_PAINT:
        break;
    case WM_CREATE:
        break;
    case WM_SIZE:
        RECT rect;
        GetWindowRect(hWnd, &rect);
        glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
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
