#include "Engine.h"
#include "Context.h"
#include "Core.h"
#include "GLInitializer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPWSTR lpCmdLine, int nCmdShow) {  
    White::Engine::Graphics::Context context; 
    context.Init();
    context.Show();
    context.Update();
    White::Engine::Graphics::Core core;
    context.SetupDemo(core);
    context.Loop(core);
    context.Destroy();
  
    return 0;
}
