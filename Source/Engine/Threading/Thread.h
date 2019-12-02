#pragma once

namespace Threading {

class Thread {
public:
    void Create();
    DWORD Execute();

private:
    static DWORD WINAPI Start(LPVOID lpParam);
};

}
