#pragma once

#include "Shader.h"

namespace White {
namespace Engine {
namespace Graphics {

struct Program {
    Program();
    ~Program();

    void Create();
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Link();
    void Use();
    void Delete();
    
    int GetVariableLocation(std::string name);
    void SetVariable(std::string name, float v0);
    void SetVariable(std::string name, float v0, float v1);
    void SetVariable(std::string name, float v0, float v1, float v2);
    void SetVariable(std::string name, float v0, float v1, float v2, float v3);
    void SetVariable(std::string name, int v0);
    void SetVariable(std::string name, int v0, int v1);
    void SetVariable(std::string name, int v0, int v1, int v2);
    void SetVariable(std::string name, int v0, int v1, int v2, int v3);
    void SetVariable(std::string name, unsigned v0);
    void SetVariable(std::string name, unsigned v0, unsigned v1);
    void SetVariable(std::string name, unsigned v0, unsigned v1, unsigned v2);
    void SetVariable(std::string name, unsigned v0, 
                     unsigned v1, unsigned v2, unsigned v3);
    void SetVariable(int location, float v0);
    void SetVariable(int location, float v0, float v1);
    void SetVariable(int location, float v0, float v1, float v2);
    void SetVariable(int location, float v0, float v1, float v2, float v3);
    void SetVariable(int location, int v0);
    void SetVariable(int location, int v0, int v1);
    void SetVariable(int location, int v0, int v1, int v2);
    void SetVariable(int location, int v0, int v1, int v2, int v3);
    void SetVariable(int location, unsigned v0);
    void SetVariable(int location, unsigned v0, unsigned v1);
    void SetVariable(int location, unsigned v0, unsigned v1, unsigned v2);
    void SetVariable(int location, unsigned v0, 
                     unsigned v1, unsigned v2, unsigned v3);

    GLuint id;
};

}
}
}
