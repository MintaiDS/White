#pragma once

#include "Model.h"

namespace White {
namespace Engine {
namespace Graphics {

class RenderData {
public:
    void AddModel(Model mode);
    std::vector<Model>& GetModels();

protected:
    std::vector<Model> models;
};

}
}
}
