#include "Engine.h"
#include "Canvas.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPWSTR lpCmdLine, int nCmdShow) {  
    White::Engine::Render::Util::Canvas canvas(hInstance);
    canvas.show();
    canvas.update();
    canvas.loop();
    canvas.destroy();
    
    return 0;
}
