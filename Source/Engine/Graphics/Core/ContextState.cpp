#include "ContextState.h"

namespace White {
namespace Engine {
namespace Graphics {

ContextState::ContextState() {}

void ContextState::SetRenderData(RenderData renderData) {
    this->renderData = renderData;
}

ModelFormat ContextState::GetModelFormat() {
    return modelFormat;
}

}
}
}
