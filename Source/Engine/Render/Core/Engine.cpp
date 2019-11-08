#include "Engine.h"
#include "Canvas.h"
#include "GLInitializer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPWSTR lpCmdLine, int nCmdShow) {  
    White::Engine::Render::Util::Canvas canvas(hInstance);
    canvas.Show();
    canvas.Update();
    canvas.Loop();
    canvas.Destroy();
    
    return 0;
}
