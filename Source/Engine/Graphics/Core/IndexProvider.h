#pragma once

#include <deque>
#include <map>
#include <iterator>

namespace White {
namespace Engine {

template<typename T = unsigned, typename U = unsigned>
class IndexProvider {
public:
    IndexProvider();

    void Add(T key, U index);
    void Remove(T key);
    U Get(T key);

protected:
    std::map<T, U> mapping;
    std::deque<U> unused;
};

template<typename T, typename U>
IndexProvider<T, U>::IndexProvider() {}

template<typename T, typename U>
void IndexProvider<T, U>::Add(T key, U index) {
    if (!unused.empty()) {
        index = unused.front();
        unused.pop_front();
    }

    mapping.insert(std::make_pair(key, index));
}

template<typename T, typename U>
void IndexProvider<T, U>::Remove(T key) {
    std::map<T, U>::iterator it = mapping.find(key);
    U index = it->second;
    mapping.erase(it);
    unused.push_back(index);
}

template<typename T, typename U>
U IndexProvider<T, U>::Get(T key) { 
    std::map<T, U>::iterator it = mapping.find(key);
    
    return it->second;
}

}
}
