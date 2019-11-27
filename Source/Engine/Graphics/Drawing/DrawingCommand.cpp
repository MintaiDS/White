#include "DrawingCommand.h"
#include "GLFunctions.h"
#include "Logger.h"

using namespace White::Util;

namespace White {
namespace Engine {
namespace Graphics {

DrawingCommand::DrawingCommand() : activeCommand(nullptr) {}

void DrawingCommand::Invoke() {
    ((*this).*(activeCommand))();
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
    Logger& logger = Logger::GetInstance();
    logger.Init("log-command.txt");
    logger << 1;
    logger << args.topology;
    logger << args.indicesCnt;
    logger << args.indexType;
    logger << args.offset;

    glDrawElements(args.topology, args.indicesCnt, 
                   args.indexType, args.offset);
}

void DrawingCommand::DrawArraysWrapper() {
    glDrawArrays(args.topology, args.first, args.count);
}

}
}
}

