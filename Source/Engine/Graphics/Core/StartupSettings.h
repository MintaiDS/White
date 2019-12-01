#pragma once

#include <windows.h>

#include <string>
#include <vector>

namespace White {
namespace Engine {

class StartupSettings {
public:
    static StartupSettings& GetInstance();
    std::vector<std::wstring> GetArgs();
    void PushArg(std::wstring arg);
    void ParseCommandLineArgs(); 
    HWND GetWindowHandle() const;
    void SetWindowHandle(HWND hWnd);

protected:
    std::vector<std::wstring> args;
    HWND hWnd;

private:
    StartupSettings();
};

}
}
