#pragma once

#include <windows.h>

namespace Threading {

class Mutex {
public:
    Mutex();

    void Create();
    void Wait();
    void Release();

protected:
    HANDLE hMutex;
};

}
