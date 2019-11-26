#pragma once

namespace White {
namespace Engine {
namespace Graphics {

class Texture : public Object {
public:
    Texture();
    
    void SetImage(unsigned image);

protected:
    unsigned image;
};

}
}
}
