#pragma once

namespace White { 
namespace Engine {
namespace Graphics { 
namespace GL {

struct GLInitializer {
    static void Init();
    static void* GetFunctionAddress(char* name);
};

}
}
}
}
