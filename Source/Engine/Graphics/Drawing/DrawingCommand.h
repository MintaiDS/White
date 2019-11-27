#pragma once

#include "ModelFormat.h"

namespace White {
namespace Engine {
namespace Graphics {

class DrawingCommand {
public:
    DrawingCommand();

    struct Arguments {
        int topology;
        int first;
        int count;
        int indicesCnt;
        int indexType;
        const void* offset;
    };

    void Invoke();
    void UpdateCommand(ModelFormat modelFormat);
    void UpdateData(Arguments args);

protected: 
    void DrawElementsWrapper();
    void DrawArraysWrapper(); 
    
    void (DrawingCommand::*activeCommand)();
    Arguments args;
};

}
}
}
