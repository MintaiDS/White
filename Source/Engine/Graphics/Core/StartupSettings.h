#pragma once

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

protected:
    std::vector<std::wstring> args;

private:
    StartupSettings();
};

}
}
