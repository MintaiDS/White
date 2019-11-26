#pragma once

#include "RenderData.h"

namespace White {
namespace Engine {
namespace Graphics {

class ContextState {
public:
    ContextState();

    void SetRenderData(RenderData renderData);
    ModelFormat GetModelFormat();

protected:
    RenderData renderData;
    ModelFormat modelFormat;
};

}
}
}
