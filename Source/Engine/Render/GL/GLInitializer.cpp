#include "GLInitializer.h"
#include "GLFunctions.h"

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

namespace White { 
namespace Engine {
namespace Render { 
namespace GL {

void GLInitializer::Init() {
    GLFunctions& gl = GLFunctions::Get();
    gl.Clear = reinterpret_cast<PFNGLCLEARPROC>
               (GetFunctionAddress("glClear"));
    gl.ClearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>
                    (GetFunctionAddress("glClearColor"));
    gl.CreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>
                      (GetFunctionAddress("glCreateShader"));
    gl.ShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>
                      (GetFunctionAddress("glShaderSource"));
    gl.CompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>
                       (GetFunctionAddress("glCompileShader"));
    gl.AttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>
                      (GetFunctionAddress("glAttachShader"));
    gl.DetachShader = reinterpret_cast<PFNGLDETACHSHADERPROC>
                      (GetFunctionAddress("glDetachShader"));
    gl.DeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>
                      (GetFunctionAddress("glDeleteShader"));
    gl.CreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>
                       (GetFunctionAddress("glCreateProgram"));
    gl.LinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>
                     (GetFunctionAddress("glLinkProgram"));
    gl.UseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>
                    (GetFunctionAddress("glUseProgram"));
    gl.DeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                       (GetFunctionAddress("glDeleteProgram"));
    gl.GenBuffers = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                    (GetFunctionAddress("glGenBuffers"));
    gl.BindBuffer = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                    (GetFunctionAddress("glBindBuffer"));
    gl.BufferData = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                    (GetFunctionAddress("glBufferData"));
    gl.VertexAttribPointer = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                             (GetFunctionAddress("glVertexAttribPointer"));
    gl.EnableVertexAttribArray = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                                 (GetFunctionAddress("glEnableVertexAttribArray"));
    gl.BufferSubData = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                       (GetFunctionAddress("glBufferSubData"));
    gl.DeleteBuffers = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                       (GetFunctionAddress("glDeleteBuffers"));
    gl.DeleteVertexArrays = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                            (GetFunctionAddress("glDeleteVertexArrays"));
    gl.UniformMatrix4fv = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                          (GetFunctionAddress("glUniformMatrix4fv"));
    gl.DrawElements = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                      (GetFunctionAddress("glDrawElements"));
    gl.Enable = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                (GetFunctionAddress("glEnable"));
    gl.CullFace = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                  (GetFunctionAddress("glCullFace"));
    gl.FrontFace = reinterpret_cast<PFNGLDELETEPROGRAMPROC>
                   (GetFunctionAddress("glFrontFace"));















}

void* GLInitializer::GetFunctionAddress(char* name) {
    PROC address = wglGetProcAddress(name);
    if (!address) {
        HMODULE hModule = LoadLibraryW(L"opengl32.dll");
        address = GetProcAddress(hModule, name);
    }

    return address;
}

}
}
}
}
