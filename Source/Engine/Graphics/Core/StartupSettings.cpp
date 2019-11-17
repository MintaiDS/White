#include "StartupSettings.h"

#include <windows.h>
#include <shellapi.h>

namespace White {
namespace Engine {

StartupSettings& StartupSettings::GetInstance() {
    static StartupSettings startupSettingsInstance;

    return startupSettingsInstance;
}

std::vector<std::wstring> StartupSettings::GetArgs() {
    return args;
}

void StartupSettings::PushArg(std::wstring arg) {
    args.push_back(arg);
}

void StartupSettings::ParseCommandLineArgs() {
    int numArgs;
    LPWSTR* argsArray = CommandLineToArgvW(GetCommandLineW(), &numArgs);
    for (int i = 0; i < numArgs; ++i) {
        args.push_back(std::wstring(argsArray[i]));
    }
}

StartupSettings::StartupSettings() {}

}
}
