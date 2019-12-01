#pragma once

namespace White {

template<typename T>
class RawDataBlock {
public:
    template<typename T>
    T* GetDestPtr(); 
    template<typename T>
    const T* GetSrcPtr();

private
    std::unique_ptr<void*> dataPtr;
};

template<typename T>
T* RawDataBlock.GetDestPtr() {
    return reinterpret_cast<T*>(dataPtr.get());
}

template<typename T>
const T* GetSrcPtr() {
    return reinterpret_cast<const T*>(dataPtr.get());
}

}
