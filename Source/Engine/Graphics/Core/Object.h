#pragma once

#include <vector>

namespace White {

class Object {
public:
    virtual ~Object();
    virtual void AddChild(unsigned child);

protected:
    std::vector<unsigned> childs;
};

}
