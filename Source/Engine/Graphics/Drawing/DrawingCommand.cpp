#include "DrawingCommand.h"
#include "GLFunctions.h"

namespace White {
namespace Engine {
namespace Graphics {

DrawingCommand::DrawingCommand() : activeCommand(nullptr) {}

void DrawingCommand::Invoke() {
    activeCommand;
}

void DrawingCommand::UpdateCommand(ModelFormat modelFormat) {
    activeCommand = &DrawingCommand::DrawArraysWrapper;
    if (modelFormat.isIndexed) {
        activeCommand = &DrawingCommand::DrawElementsWrapper;
    }
}

void DrawingCommand::UpdateData(Arguments args) {
    this->args = args;
}

void DrawingCommand::DrawElementsWrapper() {
    glDrawElements(args.topology, args.indicesCnt, 
                   args.indexType, args.offset);
}

void DrawingCommand::DrawArraysWrapper() {
    glDrawArrays(args.topology, args.first, args.count);
}

}
}
}

