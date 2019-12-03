#include "Mutex.h"

namespace Threading {

Mutext::Mutex() : hMutex(0) {}

void Mutex::Create() {
    hMutex = CreateMutexW(nullptr, false, nullptr);
}

void Mutex::Wait() {
    WaitForSingleObject(hMutex, INFINITE);
}

void Mutex::Release() {
    ReleaseMutex(hMutex);
}

}
