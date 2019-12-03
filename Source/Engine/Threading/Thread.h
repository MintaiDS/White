#pragma once

#include <windows.h>

namespace Threading {

class Thread {
public:
    Thread();

    void Create();
    DWORD Execute();

protected:
    static DWORD WINAPI Start(LPVOID lpParam);
    HANDLE hThread;
};

}
