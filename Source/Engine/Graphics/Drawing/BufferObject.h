#pragma once

#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Graphics {

class BufferObject {
public:
     void Create();
     void Bind(GLenum target);
     void SetData(GLsizeiptr size, const GLvoid* data, GLenum usage);
     void SetSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data); 
     void Delete();
  
private:
     GLuint id;
     GLenum target;
};
  
} 
} 
} 
  
  
  
  
  
  
  
  
  
  
  
  
