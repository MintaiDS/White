#include "Context.h"
#include "GLFunctions.h"
#include "GLInitializer.h"
#include "Program.h"

#include <random>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

using namespace White::Engine::Graphics::GL;

namespace White {
namespace Engine {
namespace Graphics {

Context::~Context() {
    UnregisterClassW(L"Context", GetModuleHandleW(NULL));
}

void Context::Init() {
    CreateWindowClass();
    RegisterWindowClass(); 
    CreateContextWindow();
    SetPixelFormatDescriptor();
    Create();
    SetupDemo();
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
                           WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
                           CW_USEDEFAULT, CW_USEDEFAULT, 
                           CW_USEDEFAULT, CW_USEDEFAULT, 
                           NULL, NULL, GetModuleHandleW(NULL), NULL);
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
    GLInitializer::Init(); 
}

void Context::SetupDemo() {
    GLFunctions& gl = GLFunctions::Get();
    Program program;
    Shader shader(GL_VERTEX_SHADER);
    std::wstring path = L"Engine/Shaders/default.vsh";
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    shader.Delete();
    path = L"Engine/Shaders/default.fsh";
    shader.Create(GL_FRAGMENT_SHADER);
    shader.Source(path);
    shader.Compile();
    program.Attach(shader);
    shader.Delete();
    program.Link();
    program.Use();
    program.Delete();
    GLFunctions::Get().Enable(GL_DEPTH_TEST);
    GLFunctions::Get().Enable(GL_CULL_FACE);
    GLFunctions::Get().CullFace(GL_FRONT);
    GLFunctions::Get().FrontFace(GL_CCW);
    GLfloat vertices[3][4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.0f, 0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f}
    };
    GLuint surface[3] = {
        0, 1, 2
    };
    GLuint ids[3];
    gl.GenVertexArrays(1, &ids[0]); 
    gl.BindVertexArray(ids[0]);
    gl.GenBuffers(1, &ids[1]);
    gl.BindBuffer(GL_ARRAY_BUFFER, ids[1]);
    gl.BufferData(GL_ARRAY_BUFFER, 
                  sizeof (GLfloat) * 4 * 3, nullptr, GL_STATIC_DRAW);
    gl.GenBuffers(1, &ids[2]);
    gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[2]);
    gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, 
                  sizeof surface[0] * 3, nullptr, GL_STATIC_DRAW);
    gl.VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    gl.EnableVertexAttribArray(0);
    gl.BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 
                     sizeof(GLuint) * 3, (const GLvoid*)surface);
    gl.BufferSubData(GL_ARRAY_BUFFER, 0, 
                     sizeof(GLfloat) * 4 * 3, (const GLvoid*)vertices); 
}

void Context::Show() {
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
}

void Context::Update() {
    UpdateWindow(hWnd);
}

void Context::Destroy() {
    DestroyWindow(hWnd);
}

void Context::Loop() {
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

void Context::Render() {
    std::random_device random;
    GLfloat r = 0.5f;//(random() % 1000) / 1000.0;
    GLfloat g = 0.5f;//(random() % 1000) / 1000.0;
    GLfloat b = 0.5f;//(random() % 1000) / 1000.0;
    HDC hdc = GetDC(hWnd);
    //GLInitializer::Init();
    GLFunctions& gl = GLFunctions::Get();
    gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl.ClearColor(r, g, b, 1.0f);
    // Test draw
    //Just create hello triangle      
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const GLvoid*)nullptr);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    SwapBuffers(hdc);
}

LRESULT CALLBACK Context::WindowProcCallback(HWND hWnd, UINT uMsg,
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
