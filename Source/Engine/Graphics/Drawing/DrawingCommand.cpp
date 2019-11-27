#include "DrawingCommand.h"

    glDrawElements(GL_TRIANGLES, 
                   indicesCnt, 
                   GL_UNSIGNED_INT, 
                   reinterpret_cast<const GLvoid*>(skip * sizeof(GLuint))); 

namespace White {
namespace Engine {
namespace Graphics {

DrawingCommand::DrawingCommand() : activeCommand(nullptr) {}

void DrawingCommand::Invoke() {
    activeCommand();
}

void DrawingCommand::UpdateCommand(ModelFormat modelFormat) {
    activeCommand = &DrawArraysWrapper;
    if (modelFormat.isIndexed) {
        activeCommand = &DrawElementsWrapper;
    }
}

void DrawingCommand::UpdateData(Arguments args) {
    this->args = args;
}

void DrawingCommand::DrawElementsWrapper() {
    glDrawElements(args.topology, args,indicesCnt, 
                   args.indexType, args.offset);
}

void DrawingCommand::DrawArraysWrapper() {
    glDrawArrays(args.topology, args.fist, args.count);
}

}
}
}

