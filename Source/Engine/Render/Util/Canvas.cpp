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
                  sizeof vertices[0] * 3, nullptr, GL_STATIC_DRAW);
    gl.GenBuffers(1, &ids[2]);
    gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[2]);
    gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, 
                  sizeof surface[0] * 3, nullptr, GL_STATIC_DRAW);
    gl.VertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    gl.EnableVertexAttribArray(0);
    gl.BufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 
                     sizeof(GLuint) * 3, (const GLvoid*)surface);
    gl.BufferSubData(GL_ARRAY_BUFFER, 0, 
                     sizeof(GLfloat) * 4 * 3, (const GLvoid*)vertices); 
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
