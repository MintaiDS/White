#pragma once

#include <windows.h>

#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glcorearb.h>

namespace White {
namespace Engine {
namespace Render {
namespace GL {

struct GLFunctions {
    GLFunctions(const GLFunctions& other) = delete;
    void operator=(const GLFunctions& other) = delete;
    static GLFunctions& Get();

    PFNGLCLEARPROC Clear;
    PFNGLCLEARCOLORPROC ClearColor;
    PFNGLCREATESHADERPROC CreateShader;
    PFNGLSHADERSOURCEPROC ShaderSource;
    PFNGLCOMPILESHADERPROC CompileShader; 
    PFNGLATTACHSHADERPROC AttachShader;
    PFNGLDETACHSHADERPROC DetachShader;
    PFNGLDELETESHADERPROC DeleteShader;
    PFNGLCREATEPROGRAMPROC CreateProgram;
    PFNGLLINKPROGRAMPROC LinkProgram;
    PFNGLUSEPROGRAMPROC UseProgram;
    PFNGLDELETEPROGRAMPROC DeleteProgram;
    PFNGLGENBUFFERSPROC GenBuffers;
    PFNGLBINDBUFFERPROC BindBuffer;
    PFNGLBUFFERDATAPROC BufferData;
    PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
    PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    PFNGLBUFFERSUBDATAPROC BufferSubData;
    PFNGLDELETEBUFFERSPROC DeleteBuffers;
    PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
    PFNGLENABLEPROC Enable;
    PFNGLCULLFACEPROC CullFace;
    PFNGLFRONTFACEPROC FrontFace;
    PFNGLBINDVERTEXARRAYPROC BindVertexArray;
    PFNGLGETSHADERIVPROC GetShaderiv;
    PFNGLGETPROGRAMIVPROC GetProgramiv;

private:
    GLFunctions();
};

}
}
}
}
