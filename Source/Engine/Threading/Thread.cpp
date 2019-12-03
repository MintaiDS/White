#include "Thread.h"

namespace Threading {

Thread::Thread() : hThread(0) {}

void Thread::Start(LPVOID lpParam) {
    return static_cast<Thread*>(lpParam)->Execute();
}

}
