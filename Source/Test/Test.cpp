#include <iostream>

#include "../Math/Vector.h"

int main() {
    int n;
    std::vector<int> a;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        a.push_back(x);
    }
    White::Math::Vector<int> v(a);
    White::Math::Vector<int> u(v);
    White::Math::Vector<int> p = u - v;
    u += v;
    for (int i = 0; i < u.size; i++) {
        std::cout << u[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < v.size; i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < p.size; i++) {
        std::cout << p[i] << " ";
    }
}
