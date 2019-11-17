#include "ObjectIDProvider.h"

namespace White {
namespace Engine {

unsigned ObjectIDProvider::lastID = 0;

unsigned ObjectIDProvider::GetNextID() {
    return ++lastID;
}

}
}
