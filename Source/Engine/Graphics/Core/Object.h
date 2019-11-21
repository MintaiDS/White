#pragma once

#include <vector>

namespace White {

class Object {
public:
    virtual ~Object();

protected:
    std::vector<unsigned> childs;
};

}
