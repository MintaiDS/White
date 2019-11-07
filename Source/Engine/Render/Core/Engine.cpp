#include "Engine.h"
#include "Canvas.h"

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, 
                                   WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
        break;
    case WM_CREATE:
        MessageBoxW(NULL, L"On create", NULL, MB_OK);
        break;
    case WM_SIZE:
        UpdateWindow(hWnd);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
} 


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    LPWSTR lpCmdLine, int nCmdShow) {  
    //WNDCLASSEXW wc;

    //wc.cbSize = sizeof(WNDCLASSEXW);
    //wc.style = CS_HREDRAW | CS_VREDRAW;
    //wc.lpfnWndProc = ::WindowProc;
    //wc.cbClsExtra = 0;
    //wc.cbWndExtra = 0;
    //wc.hInstance = GetModuleHandleW(NULL);
    //wc.hIcon = NULL;
    //wc.hCursor = NULL;
    //wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    //wc.lpszMenuName = NULL;
    //wc.lpszClassName = L"Canvas";
    //wc.hIconSm = NULL;

    //if (!RegisterClassExW(&wc)) {
    //    MessageBoxW(NULL, L"RegisterClassExW failure.", NULL, MB_OK);
    //}

    //HWND hWnd = CreateWindowExW(0, L"Canvas", L"White Engine",
    //                            WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
    //                            CW_USEDEFAULT, CW_USEDEFAULT, 
    //                            CW_USEDEFAULT, CW_USEDEFAULT, 
    //                            NULL, NULL, GetModuleHandleW(NULL), NULL);

    //if (hWnd == NULL) {
    //    MessageBoxW(NULL, L"Error", NULL, MB_OK);
    //}

    //ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    //UpdateWindow(hWnd);

    //MSG msg;
    //while (GetMessageW(&msg, NULL, 0, 0)) {
    //    TranslateMessage(&msg);
    //    DispatchMessageW(&msg);
    //}


    White::Engine::Render::Util::Canvas canvas(hInstance);
    canvas.show();
    canvas.update();
    canvas.loop();
    canvas.destroy();
    
    return 0;
}
