#include "Engine.h"
#include "Context.h"
#include "Core.h"
#include "GLInitializer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPWSTR lpCmdLine, int nCmdShow) {  
    White::Engine::Graphics::Core core;
    White::Engine::Graphics::Context context(core); 
    context.Init();
    context.Show();
    context.Update();
    context.Loop();
    context.Destroy();
  
    return 0;
}
