#include "Thread.h"

namespace Threading {

void Thread::Start(LPVOID lpParam) {
    return static_cast<Thread*>(lpParam)->Execute();
}

}
