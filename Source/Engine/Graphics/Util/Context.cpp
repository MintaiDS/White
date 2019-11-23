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
    //Program program;
    //Shader shader(GL_VERTEX_SHADER);
    //std::wstring path = L"Engine/Shaders/default.vsh";
    //shader.Source(path);
    //shader.Compile();
    //program.Attach(shader);
    ////shader.Delete();
    //path = L"Engine/Shaders/default.fsh";
    //shader.Create(GL_FRAGMENT_SHADER);
    //shader.Source(path);
    //shader.Compile();
    //program.Attach(shader);
    ////shader.Delete();
    //program.Link();
    //program.Use();
    ////program.Delete();
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW); 
    //Vector<GLfloat> color = {1.0f, 1.0f, 0.0f, 1.0f};
    //Vector<GLfloat> colorBorder = {1.0f, 0.0f, 1.0f, 1.0f};
    //Disk<GLfloat> disk(0.2);
    //Mesh<GLfloat> mesh = disk.ToMesh(color, 720);
    //core.renderer.AddMesh(mesh);
    //Ring<GLfloat> ring(0.1, 0.28);
    //mesh = ring.ToMesh(colorBorder, 720);
    //core.renderer.AddMesh(mesh);

    //mesh = ring1.ToMesh(color, 720);
    //core.renderer.AddMesh(mesh);


    
    //Ring<GLfloat> ring(0.4, 0.7);
    //Vector<GLfloat> color = {1.0f, 1.0f, 0.0f, 1.0f};
    //Mesh<GLfloat> mesh = ring.ToMesh(color, 360);
    //VertexArrayObject vertexArray;
    //vertexArray.Create();
    //vertexArray.Bind();
    //BufferObject arrayBuffer;
    //BufferObject elementArrayBuffer;
    //arrayBuffer.Create();
    //elementArrayBuffer.Create();
    //arrayBuffer.Bind(GL_ARRAY_BUFFER);
    //elementArrayBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
    //arrayBuffer.SetData(mesh.GetSize(), 
    //                    nullptr, 
    //                    GL_STATIC_DRAW);
    //elementArrayBuffer.SetData(sizeof(GLuint) * mesh.indices.size(), 
    //                           nullptr, 
    //                           GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 4, 
    //                      GL_FLOAT, GL_FALSE, 
    //                      sizeof(GLfloat) * 4 * 2, 
    //                      reinterpret_cast<const GLvoid*>(0));
    //glVertexAttribPointer(1, 4, 
    //                      GL_FLOAT, GL_FALSE, 
    //                      sizeof(GLfloat) * 4 * 2, 
    //                      reinterpret_cast<const GLvoid*>(sizeof(GLfloat) * 4));
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    //arrayBuffer.SetSubData(0, mesh.GetSize(), 
    //                       reinterpret_cast<const GLvoid*>(mesh.GetRawData()));
    //elementArrayBuffer.SetSubData(
    //                    0, mesh.indices.size() * sizeof(GLuint),
    //                    reinterpret_cast<const GLvoid*>(mesh.GetRawIndices()));
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
    //ShowWindow(hWnd, SW_SHOWMAXIMIZED);
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
    //GLfloat rgba[] = {0.0, 0.0, 0.0, 1.0f};
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
    //Ring<GLfloat> ring(0.4, 0.7);
    //Vector<GLfloat> color = {1.0f, 1.0f, 0.0f, 1.0f};
    //Mesh<GLfloat> mesh = ring.ToMesh(color, 360);
    //glDrawElements(GL_TRIANGLES, 360 * 2 * 6, 
    //               GL_UNSIGNED_INT, (const GLvoid*)nullptr);
}

LRESULT CALLBACK Context::WindowProcCallback(HWND hWnd, UINT uMsg,
                                             WPARAM wParam, LPARAM lParam) {   
    HDC hdc;
    HGLRC hglrc; 

    //return 0;
    switch (uMsg) {
    case WM_PAINT:
        break;
    case WM_CREATE:
        //MessageBox(NULL, NULL, NULL, MB_OK);
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
