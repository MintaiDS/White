#include "Engine.h"
#include "Context.h"
#include "GLInitializer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPWSTR lpCmdLine, int nCmdShow) {  
    White::Engine::Graphics::Context context;
    context.Init();
    context.Show();
    context.Update();
    context.Loop();
    context.Destroy();
  
    return 0;
}
