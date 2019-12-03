#pragma once

#include <windows.h>

namespace Threading {

template<typename T>
class Thread {
public:
    Thread();

    void Create();
    void Start(T* obj, DWORD (T::*ExecuteFunction)());

protected: 
    DWORD ExecuteThread();
    static DWORD WINAPI StartThread(LPVOID lpParam);
    
    T* obj;
    DWORD (T::*ExecuteFunction)();
    DWORD id;
    HANDLE hThread;
};

template<typename T>
Thread<T>::Thread() :
        hThread(0)
        , id(0)
        , obj(nullptr)
        , ExecuteFunction(nullptr) {}

template<typename T>
void Thread<T>::Create() {}

template<typename T>
void Thread<T>::Start(T* obj, DWORD (T::*ExecuteFunction)()) {
    this->obj = obj;
    this->ExecuteFunction = ExecuteFunction;
    CreateThread(nullptr, 0, StartThread, static_cast<void*>(this), 0, &id); 
}

template<typename T>
DWORD Thread<T>::ExecuteThread() {
     
    return 0;
}

template<typename T>
DWORD Thread<T>::StartThread(LPVOID lpParam) {
    Thread<T>* threadObj = static_cast<Thread<T>*>(lpParam);

    return (threadObj->obj->*threadObj->ExecuteFunction)();
}

}
