#include "Context.h"
#include "GLFunctions.h"
#include "GLInitializer.h"
#include "VertexArrayObject.h"
#include "Program.h"
#include "BufferObject.h"
#include "VertexAttribute.h"
#include "VertexData.h"
#include "Mesh.h"

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
                           WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_VISIBLE,
                           100, 100,  
                           500, 500, 
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
    LoadFunctions();
}

void Context::SetupDemo() {
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    Vector<GLfloat> positions[3] = {
        {-0.5f, -0.5f, 0.1f, 1.0f},
        {0.0f, 0.5f, 0.1f, 1.0f},
        {0.5f, -0.5f, 0.1f, 1.0f}
    }; 
    Vector<GLfloat> colors[3] = {
        {1.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f}
    }; 
    VertexAttribute<GLfloat> attributes[3][2] = {
        {positions[0], colors[0]},
        {positions[1], colors[1]},
        {positions[2], colors[2]}
    };
    std::vector<VertexData<GLfloat>> verts;
    for (int i = 0; i < 3; i++) {
        std::vector<VertexAttribute<GLfloat>> attribs;
        attribs.push_back(attributes[i][0]);
        attribs.push_back(attributes[i][1]);
        VertexData<GLfloat> data(attribs);
        verts.push_back(data);
    } 
    Mesh<GLfloat> mesh(verts);
    GLfloat* ptr = mesh.GetRawData();
    //std::ofstream out("log.txt");
    //for (int i = 0; i < 3; i++) {
    //    for (int j = 0; j < 8; j++) {
    //        out << ptr[i][j] << " ";
    //    }
    //    out << std::endl;
    //}
    //out.close();
    GLfloat vertices[3][8] = {
        {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f}
    };
    GLuint surface[3] = {
        0, 1, 2
    };
    VertexArrayObject vertexArray;
    vertexArray.Create();
    vertexArray.Bind();
    BufferObject arrayBuffer;
    BufferObject elementArrayBuffer;
    arrayBuffer.Create();
    elementArrayBuffer.Create();
    arrayBuffer.Bind(GL_ARRAY_BUFFER);
    elementArrayBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
    arrayBuffer.SetData(mesh.GetSize(), 
                        nullptr, 
                        GL_STATIC_DRAW);
    elementArrayBuffer.SetData(sizeof(GLuint) * 3, 
                               nullptr, 
                               GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
                          sizeof(GLfloat) * 4 * 2, reinterpret_cast<const GLvoid*>(0));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 
                          sizeof(GLfloat) * 4 * 2, 
                          reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * 4));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    arrayBuffer.SetSubData(0, mesh.GetSize(), 
                           reinterpret_cast<const GLvoid*>(mesh.GetRawData()));
    elementArrayBuffer.SetSubData(0, sizeof(GLuint) * 3,
                                  reinterpret_cast<const GLvoid*>(surface));
    //glBindBuffer(GL_ARRAY_BUFFER, ids[1]);
    //glBufferData(GL_ARRAY_BUFFER, 
    //             sizeof (GLfloat) * 4 * 3, nullptr, GL_STATIC_DRAW);
    //glGenBuffers(1, &ids[2]);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[2]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
    //             sizeof surface[0] * 3, nullptr, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(0);
    //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 
    //                sizeof(GLuint) * 3, (const GLvoid*)surface);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, 
    //                sizeof(GLfloat) * 4 * 3, (const GLvoid*)vertices); 
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
    GLfloat rgba[] = {0.0, 0.0, 0.0, 1.0f};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const GLvoid*)nullptr);
    SwapBuffers(GetDC(hWnd));
    Sleep(30);
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
