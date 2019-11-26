#pragma once

namespace White {
namespace Engine {

class ObjectIDProvider {
public:
    static unsigned GetNextID();

protected:
    static unsigned lastID;
};

}
}
