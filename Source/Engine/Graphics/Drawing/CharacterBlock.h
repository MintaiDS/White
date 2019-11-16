#pragma once

#include "DigitMeshCreator.h"
#include "IDrawable.h"
#include "Grid.h"

namespace White {
namespace Engine {
namespace Graphics {

class CharacterBlock : public IDrawable<float> {
public:
    CharacterBlock(const Vector<float>& blockPosition,
                   const Vector<float>& cellSize, std::string str = "");

    std::vector<Mesh<float>>& GetMeshes();

    virtual void Translate(const Vector<GLfloat>& translation);
    virtual void Rotate(const Vector<GLfloat>& rotation);
    virtual void Scale(const Vector<GLfloat>& scaling); 

protected:
    Vector<GLfloat> translation;
    Vector<GLfloat> rotation;
    Vector<GLfloat> scaling;
    std::unique_ptr<Grid> grid;
    std::vector<Mesh<float>> meshes;
    std::string str;
};

}
}
}
